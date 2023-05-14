#include <iostream> 
#include <fstream> 
#include <string> 
#include <vector> 
using namespace std; 
 
string getInfo(vector<string> files, string path) 
{ 
    string info = ""; 
 
    for (string i : files) 
    { 
        ifstream file(i, ios::binary); 
        if (!file) break; 
         
        file.seekg(0, ios_base::end); 
        int fileSize = (int)file.tellg(); 
        string strFileSize = to_string(fileSize); 
 
        string fileName = i.substr(i.find_last_of('/') + 1, i.size()); 
 
        file.close(); 
 
        info += strFileSize + "||" + fileName + "||"; 
    } 
    string totalSize = to_string(info.length() + 2); 
    if (totalSize.length() < 5) totalSize = string(5 - totalSize.length(), '0') + totalSize; 
 
    return totalSize + "||" + info; 
} 
 
void pack(vector<string> files, string path) 
{ 
    ofstream archive(path + "/arch.arch", ios::binary); 
     
    archive << getInfo(files, path);  
 
    char byte[1]; 
    for (string i : files)  
    { 
        ifstream file(i, ios::binary);  
        if (!file) break;  
 
        while (!file.eof())  
        { 
            file.read(byte, 1); 
            archive.write(byte, 1); 
        } 
 
        cout << i << " was added to the archive." << endl; 
 
        file.close();  
    } 
    archive.close();  
} 
 
void unpack(vector<string> archives, string path) 
{ 
    for (string arcivePath : archives) 
    { 
        ifstream archive(arcivePath, ios::binary); 
 
        char charTotalSize[5]; 
        archive.read(charTotalSize, 5); 
        int totalSize = atoi(charTotalSize); 
 
        char* charInfo = new char[totalSize + 1]; 
        archive.read(charInfo, totalSize); 
        charInfo[totalSize] = '\0'; 
        string info(charInfo); 
        delete[] charInfo; 
                 
        vector<string> files; 
        vector<int> sizes; 
        int numFiles = 0; 
        while (info.find("||") != info.rfind("||")) 
        { 
            info = info.erase(0, info.find("||") + 2); 
            if (numFiles % 2 == 0) 
                sizes.push_back(stoi(info.substr(0, info.find("||")))); 
            else 
                files.push_back(info.substr(0, info.find("||"))); 
            numFiles++; 
            info = info.erase(0, info.find("||")); 
        } 
        numFiles /= 2;  
 
        char byte[1]; 
        for (int i = 0; i < numFiles; i++) 
        { 
            string filePath = path + "/" + files[i]; 
            cout << files[i] << " was unpacked." << endl; 
 
            ofstream file(filePath, ios::binary); 
            for (int j = 0; j < sizes[i] + 1; j++) 
            { 
                archive.read(byte, 1); 
                file.write(byte, 1); 
            } 
            file.close(); 
        } 
        archive.close(); 
    } 
} 
 
int main() 
{ 
    string operation = ""; 
    vector<string> files; 
    string path = ""; 
    int numFiles; 
    string file; 
 
    cout << "Pack/Unpack: "; 
    cin >> operation; 
 
    cout << "NumFiles: "; 
    cin >> numFiles; 
    for (int i = 0; i < numFiles; i++) 
    { 
        cout << "File " << i + 1 << ": "; 
        cin >> file; 
        files.push_back(file); 
    } 
 
    cout << "path: "; 
    cin >> path; 
 
    if (operation == "pack") pack(files, path); 
    if (operation == "unpack") unpack(files, path); 
}
