//Brenan Marenger
//Permission code: https://stackoverflow.com/questions/10323060/printing-file-permissions-like-ls-l-using-stat2-in-c

#include<iostream>
#include<dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include<assert.h>
#include <stdio.h>
#include <vector>

using namespace std;

int size = 0;
vector<int> inode;
bool flag = false;

void recDir(const char *path) { 
	DIR* dir;
	struct dirent* ent;
	struct stat data;

	assert((opendir(path)) != NULL);
	if ((opendir(path)) != NULL) { //opens
		auto dir = opendir(path);
		while ((ent = readdir(dir)) != NULL) { //reads each entry

			string fullPath = path;
			fullPath += '/';
			fullPath += ent->d_name;
			cout << fullPath << " ";

			if (ent->d_type == DT_DIR) { //directory
				cout << "[D]" << endl;
				if (ent->d_name[0] != '.') {
					recDir(fullPath.c_str());
				}
			}
			else if (ent->d_type == DT_REG) { //file
				if (stat(fullPath.c_str(), &data) != -1) {
					cout << "[F, " << data.st_size << "]";

					for (auto i : inode) {
						if (inode[i] == data.st_ino) {
							flag = true; 
						}
					}
					inode.push_back(data.st_ino); //prevent links repeating total size
					if (flag != true) {
						size += data.st_size;
					}
					flag = false;

					printf(" Permissions: ");
					printf((S_ISDIR(data.st_mode)) ? "d" : "-");
					printf((data.st_mode & S_IRUSR) ? "r" : "-");
					printf((data.st_mode & S_IWUSR) ? "w" : "-");
					printf((data.st_mode & S_IXUSR) ? "x" : "-");
					printf((data.st_mode & S_IRGRP) ? "r" : "-");
					printf((data.st_mode & S_IWGRP) ? "w" : "-");
					printf((data.st_mode & S_IXGRP) ? "x" : "-");
					printf((data.st_mode & S_IROTH) ? "r" : "-");
					printf((data.st_mode & S_IWOTH) ? "w" : "-");
					printf((data.st_mode & S_IXOTH) ? "x" : "-");
					printf("\n");
				}
				else if (stat(fullPath.c_str(), &data) == -1) {
					perror("read file was bad: ");
					exit(1);
				}
			}
			else if (ent->d_type == DT_LNK) {//link
				char buf[150];
				auto ret = readlink(fullPath.c_str(), buf, 150);
				if (ret == -1) {
					perror("read link was bad: ");
					exit(1);
				}
				else {
					cout << "[L" << "->";
					for (int i = 0; i < 150; i++) { //finding link to
						if (buf[i] > 0) {
							cout << buf[i];
						}
						else {
							break;
						}
					}
					cout << "]"<< endl;
				}
			}
		}
		closedir(dir);
	}
	else {
		perror("Error opening");
	}
}

int main(int argc, char** argv) {
	recDir(argv[1]);
	cout << "total space used: " << size << endl;
	return 0;
}