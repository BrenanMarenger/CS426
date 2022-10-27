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
#include<cstring>
using namespace std;

int size = 0;
vector<int> inode;


bool checkInode(int node) { //return true if already saw file
	for (auto i : inode) {
		if (inode[i] == node) {
			return true;
		}
	}
	inode.push_back(node);
	return false;
}

void recDir(const char *path) { 

	DIR* dir;
	struct dirent* ent;
	struct stat data;

	if ((dir = opendir(path)) != NULL) { //opens
		assert(dir != nullptr);
		while ((ent = readdir(dir)) != NULL) { //reads each entry
			string fullPath = path;
			fullPath += '/';
			fullPath += ent->d_name;
			cout << fullPath << " ";
			if (ent->d_type == DT_DIR) { //directory
				cout << "[D]" << endl;
				if ((strcmp(ent->d_name, ".")) && (strcmp(ent->d_name, ".."))) {
					recDir(fullPath.c_str());
				}
			}
			else if (ent->d_type == DT_REG) { //file
				if (stat(fullPath.c_str(), &data) != -1) {
					cout << "[F, " << data.st_size << "]";
		
					/*
					if (checkInode(data.st_ino) == false) { //check for soft link
						size += data.st_size;
					}
					*/
					
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
			}
			else if (ent->d_type == DT_LNK) {//link
				char buf[100];
				auto ret = readlink(fullPath.c_str(), buf, 100);
				assert(ret != -1);		
				buf[ret] = 0;
				cout << "[L" << "->" << buf << "]"<< endl;	
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
