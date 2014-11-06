//
//  main.cpp
//  rc4
//
//  Created by cnd9 on 14-7-3.
//  Copyright (c) 2014年 d9. All rights reserved.
//

#include <iostream>
#include <fstream>

#include "rc4.h"

#include <sys/stat.h>//chmod(,)
#include "dirent.h"
#include <time.h>

#define pcout(A) std::cout<<#A <<":"<<(A)<<std::endl

#ifdef _WIN32
#	define _file	32768
#	define _folder	16384
#else
#	define _file	8
#	define _folder	4
#endif

using namespace std;
std::string outdir;
std::string in_dir;
string& replace005C(string& c)
{
	while (c.find("\\") < c.size())
	{
		c.replace(c.find("\\"), 1, "/");
	}
	return c;
}

bool make_dir(string dir)
{
#if _WIN32
    CreateDirectory(dir.c_str(), 0);
#else
    mkdir(dir.c_str(), 0);
    chmod(dir.c_str(), 0777);
#endif
    
    return true;
}

bool fileDecode(const string infilename)
{

	string outfilename = infilename;
    outfilename.replace(0, in_dir.size(), outdir);
	pcout(outfilename.c_str());

    FILE *fi = fopen(infilename.c_str(), "rb");
    FILE *fo = fopen(outfilename.c_str(),"wb");

    fseek(fi,0,SEEK_END);
    unsigned long pSize = ftell(fi);
    unsigned char* pBuffer = new unsigned char[pSize];

    fseek(fi,0,SEEK_SET);
    pSize = fread(pBuffer,sizeof(unsigned char), pSize,fi);

    if(     infilename.find(".mp3"  )>=infilename.size()
//         && infilename.find(".plist"  )>=infilename.size()
       )
    {
        rc4::rc4_crypt(pBuffer, pSize);
    }
    fwrite(pBuffer, pSize, 1, fo);
	fflush(fi);
	
    delete []pBuffer;
    fclose(fo);
    fclose(fi);

    return true;
}
void dothis(const char* path)
{
	DIR *dir = opendir(path);
	struct dirent *ptr = 0;
	if (dir)
		ptr = readdir(dir);
	else
    {
		outdir = in_dir + "_rc4" + in_dir.substr(in_dir.find_last_of('/'), in_dir.size());
        fileDecode(path);
        return;
    }
    //is a dir
	while (ptr != NULL)
	{
        std::cout<< (int)(ptr->d_type) << ":" << path << '/' << ptr->d_name << std::endl;
		if ( ptr->d_type == _folder && strcmp(ptr->d_name, "..") != 0 && strcmp(ptr->d_name, ".") != 0 && (ptr->d_name[0]!= '.'))
		{
			string tmpoutdir = string(path) + "/" + ptr->d_name;
			tmpoutdir.replace(0, in_dir.size(), outdir);
			std::cout << tmpoutdir.c_str() << std::endl;
			make_dir(tmpoutdir);

			dothis((string(path) + "/" + ptr->d_name).c_str());
		}
		else if ( ptr->d_type == _file )
		{
			string name = string(path) + "/" + ptr->d_name;
			fileDecode(name);
		}
        ptr = readdir(dir);
	}
	closedir(dir);

}

int main(int argc, const char * argv[])
{
    if(argc < 2)
        printf("Usage:\n"
               "    ./rc4 workingDdir|workingFILE list.\n");

    unsigned long t = clock() ;
    string key = "www.gamed9.mengguo.com";
    rc4::rc4_init((unsigned char*)(key.c_str()), key.size());
    
    if (argc > 1 )for(int i = 1; i < argc; ++i)
	{
		in_dir = argv[i];
		replace005C(in_dir);
        outdir = in_dir + "_rc4";//        "/data";
        make_dir(outdir);
        dothis(in_dir.c_str());
	}
    else//worke with ./data
    {
        string rootdir(argv[0]);
		replace005C(rootdir);
		in_dir = rootdir.substr(0, rootdir.find_last_of('/')) + "/data";
		if (opendir(in_dir.c_str()))
		{
			make_dir(in_dir + "_rc4");
			make_dir(in_dir + "_rc4/data");
			outdir = in_dir + "_rc4/data";
			dothis(in_dir.c_str());
		}
    }
    std::cout << "total time:"<< (clock() - t)/1000000.0 << endl;
  
    return 0;
}

