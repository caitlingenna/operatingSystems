/* Recursive Directory Listing -- dirls
 * Uses the FileSystem Interface, take arguments from the command line (options and directories) and simulate directory listing.
 */

#include <unistd.h>
#include <iomanip>
#include <pwd.h>
#include <limits.h>
#include <grp.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>
#include <string>
using namespace std;

class DirectArgs
{ //object to hold options and associated directory given from command line arguments
  //assumes user is following usage instructions correctly, i.e. passing directory names/paths (or nothing at all), not file names
	public:
	int flags[5];
	string dirName;

	DirectArgs(int flg[5], string dName = ".")
	{
		for(int i = 0; i < 5; i++)
		{
	  		flags[i] = flg[i];
		}
		dirName = dName;
	}

	void List();	
};
void DirectArgs:: List()
{
		DIR *direc;
		struct dirent *d;
		direc = opendir(dirName.c_str());
		struct stat fileStats;
		struct passwd *ps;
		struct group *grp;
		int mode;
		char *resolvedPath;
		string n;


		if (flags[4] == 1) // -h flag, any directory given after it will just be listed by name
		{
			cout << "Usage: dirls [(-[ladfh]) (dir)]*" << endl;
        		cout << "-l: long form" << endl;
			cout << "-a: include dot files" << endl;
			cout << "-d: only this directory" << endl;
			cout << "-f: follow symbolic links" << endl;
			cout << "-h: prints this message" << endl;
        		cout << endl;
		}
		while((d = readdir(direc)))
		{
			n = d->d_name;
			lstat(d->d_name, &fileStats);
			mode = fileStats.st_mode;
		
			if(flags[2] == 1 && (n.at(0) != '.' || n.length() != 1)){} //in case of -d flag, only include this directory
			else if((flags[1] == 1 || n.at(0) != '.') || (flags[2] == 1 && n.at(0) == '.')) //-d flag overrides -a flag
			{
				if(flags[0] == 1)
				{ //if -l is enabled, print permissions, file size, user name, group name
					if(S_IRUSR & mode)
						cout << "r";
					else
						cout << "-";
					if(S_IWUSR & mode)
						cout << "w";
					else
						cout << "-";
					if(S_IXUSR & mode)
						cout << "x";
					else
						cout << "-";
					if(S_IRGRP & mode)
						cout << "r";
					else
						cout << "-";
					if(S_IWGRP & mode)
						cout << "w";
					else
						cout << "-";
					if(S_IXGRP & mode)
						cout << "x";
					else
						cout << "-";
					if(S_IROTH & mode)
						cout << "r";
					else
						cout << "-";
					if(S_IWOTH & mode)
						cout << "w";
					else
						cout << "-";
					if(S_IXOTH & mode)
						cout << "x";
					else
						cout << "-";
					 cout << " ";
					 cout << fixed;
					 cout << setw(10) << left << fileStats.st_size << " "; // print file size
					 ps = getpwuid(fileStats.st_uid); // get userid
					 grp = getgrgid(fileStats.st_gid); // get groupid
					 cout << setw(10) << left << ps->pw_name << " " << setw(10) << grp->gr_name << " ";
				}
				if(d->d_type == DT_LNK && flags[3] == 1) //if -f flag enabled, follow symbolic links
				{
					char *resolved = realpath(d->d_name, resolvedPath); 
					if(resolved) //if path was found, print path name
					{
						string n(resolved);
						cout << setw(10) <<left << n;
						cout << endl;
			
					}
					else //if path was unresolved, leave blank
						cout << endl;
				}
				 else //if -f not enabled, print original link name
				 	cout << setw(10) << left << n << endl;
			}
		}
	closedir(direc);
	cout << endl;
};

int main (int argc, char **argv)
{
	int flg[5] = {0,0,0,0,0}; // {l, a, d, f, h}
	int opt, tempOptind = 1;
	bool optGiven = false;
	string dName;
	
	while((opt = getopt(argc, argv, "ladfh")) != -1)
	{  
		optGiven = true;
		switch(opt)
		{ //flag all options given in string
			case 'l':
				flg[0] = 1;
				break;
			case 'a':
				flg[1] = 1;
				break;
			case 'd':
				flg[2] = 1;
				break;
			case 'f':
				flg[3] = 1;
				break;
			case 'h':
				flg[4] = 1;
				break;
		}
		if(argc == 2)
		{ //case of only one argument, must be an option (or wouldn't be in this while loop)
			DirectArgs direc(flg, ".");
			direc.List();
		}
		else if(tempOptind != optind)
		{ //if an option has been read
 			if(optind >= argc)
                        { //if last argument is an option with no directory given
                                DirectArgs direc(flg, ".");
                                direc.List();
                        }
			else if(argv[optind][0] == '-')
			{ //handles case of no directory following option command (but not last argument)
				DirectArgs direc(flg, ".");
				direc.List();
				for(int i = 0; i < 5; i++) //reset flags
					flg[i] = 0;
			}
			else
			{  //handles case of multiple directories given in a row
			       int o = optind;
			       for(o; argc > o; o++)
			       {	
					if(argv[o][0] == '-') //if an option is reached
						break;
					else
					{	//use same flags for each directory given
						DirectArgs direc(flg, argv[o]);
						direc.List();
					}
				}
			       for(int i = 0; i< 5; i++)
				       flg[i] = 0;
		
			}
		}
		tempOptind = optind;

	};
	if(!optGiven)
	{//handles case where only a directory is given, no options
		flg[0] = 1;
		for(int i = 1; i < argc; i++)
		{
			string n = argv[i];
			DirectArgs direc(flg, n);
	       		direc.List();
		}
      }	       

}
