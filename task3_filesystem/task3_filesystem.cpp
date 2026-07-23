// TASK 3: File System Operations and Security
// A tiny in-memory file system with:
//   - create / read / write / delete
//   - user authentication (password is HASHED, never stored as plain text)
//   - permissions (owner vs. everyone else)
//   - encryption / decryption of file contents
//   - an audit log (who did what, and whether it was allowed)
//
// Compile with:  g++ -std=c++20 task3_filesystem.cpp -o task3
// Run with:      ./task3


#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <ctime>

// User 
struct User
{
    int id;
    std::string username;
    size_t passwordHash; // NEVER store the real password -- only its hash

    User(int id, std::string username, const std::string& password)
        : id(id), username(std::move(username))
    {
        passwordHash = std::hash<std::string>{}(password);
    }

    bool authenticate(const std::string& password) const
    {
        return std::hash<std::string>{}(password) == passwordHash;
    }
};

// File 
struct File
{
    std::string name;
    std::string data;
    int ownerId;
    bool encrypted = false;

    // Simple Unix-style permissions: owner vs. everyone else.
    bool ownerCanRead = true, ownerCanWrite = true;
    bool othersCanRead = false, othersCanWrite = false;

    bool canRead(int userId)  const { return userId == ownerId ? ownerCanRead  : othersCanRead; }
    bool canWrite(int userId) const { return userId == ownerId ? ownerCanWrite : othersCanWrite; }

    void encrypt()
    {
        if (encrypted) return;
        for (char& c : data) c ^= 42; // XOR every byte with the same key
        encrypted = true;
    }

    void decrypt()
    {
        if (!encrypted) return;
        for (char& c : data) c ^= 42; // XOR-ing twice with the same key undoes it
        encrypted = false;
    }
};

// FileSystem 
class FileSystem
{
public:
    void createFile(const User& user, const std::string& name)
    {
        File f;
        f.name = name;
        f.ownerId = user.id;
        files.push_back(f);
        log(user.username, "CREATE", name, true);
    }

    bool writeFile(const User& user, const std::string& name, const std::string& data)
    {
        File* f = find(name);
        if (!f) { log(user.username, "WRITE", name, false); return false; }

        if (!f->canWrite(user.id)) { log(user.username, "WRITE", name, false); return false; }

        f->data = data;
        log(user.username, "WRITE", name, true);
        return true;
    }

    // returns "" if not allowed; sets outAllowed to true/false
    std::string readFile(const User& user, const std::string& name, bool& outAllowed)
    {
        File* f = find(name);
        if (!f) { outAllowed = false; log(user.username, "READ", name, false); return ""; }

        if (!f->canRead(user.id)) { outAllowed = false; log(user.username, "READ", name, false); return ""; }

        outAllowed = true;
        log(user.username, "READ", name, true);

        std::string result = f->data;
        if (f->encrypted)
        {
            f->decrypt();
            result = f->data;
            f->encrypt(); // put it back the way it was stored
        }
        return result;
    }

    bool deleteFile(const User& user, const std::string& name)
    {
        for (auto it = files.begin(); it != files.end(); ++it)
        {
            if (it->name == name)
            {
                if (!it->canWrite(user.id)) { log(user.username, "DELETE", name, false); return false; }
                files.erase(it);
                log(user.username, "DELETE", name, true);
                return true;
            }
        }
        log(user.username, "DELETE", name, false);
        return false;
    }

    File* find(const std::string& name)
    {
        for (auto& f : files) if (f.name == name) return &f;
        return nullptr;
    }

    void printAuditLog() const
    {
        std::cout << "\n===== AUDIT LOG =====\n";
        for (const auto& entry : auditLog) std::cout << entry << "\n";
    }

private:
    std::vector<File> files;
    std::vector<std::string> auditLog;

    void log(const std::string& username, const std::string& action, const std::string& fileName, bool allowed)
    {
        auto now = std::chrono::system_clock::now();
        std::time_t t = std::chrono::system_clock::to_time_t(now);
        std::string timeStr = std::ctime(&t);
        if (!timeStr.empty() && timeStr.back() == '\n') timeStr.pop_back();

        auditLog.push_back("[" + timeStr + "] user=" + username + " action=" + action +
                            " file=" + fileName + " result=" + (allowed ? "ALLOWED" : "DENIED"));
    }
};
// main function
int main()
{
    User alice(1, "alice", "password123");
    User bob(2, "bob", "hunter2");

    std::cout << "alice login with correct password: " << alice.authenticate("password123") << "\n";
    std::cout << "alice login with wrong password:    " << alice.authenticate("wrongpass") << "\n\n";

    FileSystem fs;
    fs.createFile(alice, "alice_notes.txt");
    fs.writeFile(alice, "alice_notes.txt", "these are alice's private notes");

    File* f = fs.find("alice_notes.txt");
    if (f) f->encrypt(); // stored encrypted at rest

    bool allowed;
    std::cout << "bob tries to read alice's file...\n";
    fs.readFile(bob, "alice_notes.txt", allowed);
    std::cout << "  allowed? " << allowed << "\n\n";

    std::cout << "alice reads her own file...\n";
    std::string content = fs.readFile(alice, "alice_notes.txt", allowed);
    std::cout << "  allowed? " << allowed << ", content: \"" << content << "\"\n";

    fs.printAuditLog();
    return 0;
}
