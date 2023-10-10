/*
 * shell.C -- CEG433 File Sys Project shell
 * pmateti@wright.edu
 */

#include "fs33types.hpp"


extern MountEntry *mtab;
extern VNIN cwdVNIN;
FileVolume * fv;                // Suspicious!
Directory * wd;                 // Suspicious!

#define nArgsMax 10
char types[1+nArgsMax];		// +1 for \0

/* An Arg-ument for one of our commands is either a "word" (a null
 * terminated string), or an unsigned integer.  We store both
 * representations of the argument. */

class Arg {
public:
  char *s;
  uint u;
} arg[nArgsMax];

uint nArgs = 0;

uint TODO()
{
  printf("to be done!\n");
  return 0;
}

uint TODO(char *p)
{
  printf("%s to be done!\n", p);
  return 0;
}

uint isDigit(char c)
{
  return '0' <= c && c <= '9';
}

uint isAlphaNumDot(char c)
{
  return c == '.' || 'a' <= c && c <= 'z'
      || 'A' <= c && c <= 'Z' || '0' <= c && c <= '9';
}

int toNum(const char *p)
{
  return (p != 0 && '0' <= *p && *p <= '9' ? atoi(p) : 0);
}

SimDisk * mkSimDisk(byte *name)
{
  SimDisk * simDisk = new SimDisk(name, 0);
  if (simDisk->nSectorsPerDisk == 0) {
    printf("Failed to find/create simDisk named %s\n", name);
    delete simDisk;
    simDisk = 0;
  }
  return simDisk;
}

void doMakeDisk(Arg * a)
{
  SimDisk * simDisk = mkSimDisk((byte *) a[0].s);
  if (simDisk == 0)
    return;
  printf("new SimDisk(%s) = %p, nSectorsPerDisk=%d,"
	 "nBytesPerSector=%d, simDiskNum=%d)\n",
	 simDisk->name, (void*) simDisk, simDisk->nSectorsPerDisk,
	 simDisk->nBytesPerSector, simDisk->simDiskNum);
  delete simDisk;
}

void doWriteDisk(Arg * a)
{
  SimDisk * simDisk = mkSimDisk((byte *) a[0].s);
  if (simDisk == 0)
    return;
  char *st = a[2].s;		// arbitrary word
  if (st == 0)			// if it is NULL, we use ...
    st = "CEG433/633/Mateti";
  char buf[1024];		// assuming nBytesPerSectorMAX < 1024
  for (uint m = strlen(st), n = 0; n < 1024 - m; n += m)
    memcpy(buf + n, st, m);	// fill with several copies of st
  uint r = simDisk->writeSector(a[1].u, (byte *) buf);
  printf("write433disk(%d, %s...) == %d to Disk %s\n", a[1].u, st, r, a[0].s);
  delete simDisk;
}

void doReadDisk(Arg * a)
{
  SimDisk * simDisk = mkSimDisk((byte *) a[0].s);
  if (simDisk == 0)
    return;
  char buf[1024];		// assuming nBytesPerSectorMAX < 1024
  uint r = simDisk->readSector(a[1].u, (byte *) buf);
  buf[10] = 0;			// sentinel
  printf("read433disk(%d, %s...) = %d from Disk %s\n", a[1].u, buf, r, a[0].s);
  delete simDisk;
}

void doQuit(Arg * a)
{
  exit(0);
}

void doEcho(Arg * a)
{
  printf("%s#%d, %s#%d, %s#%d, %s#%d\n", a[0].s, a[0].u,
	 a[1].s, a[1].u, a[2].s, a[2].u, a[3].s, a[3].u);
}

void doMakeFV(Arg * a)
{
  SimDisk * simDisk = mkSimDisk((byte *) a[0].s);
  if (simDisk == 0)
    return;
  fv = simDisk->make33fv();
  printf("make33fv() = %p, Name == %s, Disk# == %d\n",
	 (void*) fv, a[0].s, simDisk->simDiskNum);

  if (fv) {
      wd = new Directory(fv, 1, 0);
      cwdVNIN = mkVNIN(simDisk->simDiskNum, 1);
  }
}

void doCopyTo(byte* from, byte* to)
{
  uint r = fv->write33file(to, from);
  printf("write33file(%s, %s) == %d\n", to, from, r);
}

void doCopyFrom(byte* from, byte* to)
{
  uint r = fv->read33file(to, from);
  printf("read33file(%s, %s) == %d\n", to, from, r);
}

void doCopy33(byte* from, byte* to)
{
  uint r = fv->copy33file(to, from);
  printf("copy33file(%s, %s) == %d\n", to, from, r);
}

void doCopy(Arg * a)
{
  byte* to = (byte *) a[0].s;
  byte* from = (byte *) a[1].s;
  
  if (a[0].s[0] == '@' && a[1].s[0] != '@') {
    doCopyTo(from, (to + 1));
  }
  else if (a[0].s[0] != '@' && a[1].s[0] == '@') {
    doCopyFrom((from + 1), to);
  }
  else if (a[0].s[0] != '@' && a[1].s[0] != '@') {
    doCopy33(from, to);
  }
  else {
    puts("Wrong arguments to cp.");
  }
  
  //Set the link count of any copy as 1
  uint in = wd->iNumberOf(from);
  fv->inodes.setLnCnt(in,1);
  
}




void doLsLong(Arg * a)
{
  printf("\nDirectory listing for disk %s, cwdVNIN == 0x%0lx begins:\n",
	 wd->fv->simDisk->name, (ulong) cwdVNIN);
  wd->ls();                     // Suspicious!
  printf("Directory listing ends.\n");
}




//Print contents of a certain directory
void doLsofDir(Arg * a)
{
  //Absolute path
  if(a[0].s[0]=='/')
  {
    //Create a temporary directory, and a placeholder of the cwd
    Directory* tempD = new Directory(fv,1,0);
    uint pin = wd->iNumberOf((byte *)"..");
    Directory* ph = new Directory(fv,wd->nInode,pin);
    
    char loc[50];
    int i=1;
    int j=0;
    bool done = false;
    bool valid = true;
    
    //Check that we are done with the filepath and that each entered directory is a valid one to enter
    while(!done && valid)
    {
      //Make sure that we are retrieving one directory at a time
      //Store the input into a character array
      while(a[0].s[i]!='/' && ((a[0].s[i]<123 && a[0].s[i]>96) || (a[0].s[i]<91 && a[0].s[i]>64)))
      {
        loc[i-1] = a[0].s[i];
        i++;
      }
      char* next = (char*)malloc(50);;
      next[0] = 0;
      
      //Move the contents of the character array into a pointer
      for(int k=0; j<i-1; k++)
      {
        next[k] += (char)loc[j];
        j++;
      }
      j++;
      next[j]=0;
      
      //Retrive the inode of the resulting directory
      uint in = tempD->iNumberOf((byte*)next);
      
      //Wipe the character pointer for the next directory in the filepath
      for(int k=0; k<i; k++)
      {
        next[k] = 0;
      }
      
      //Check if the directory exists from the cwd
      if(in)
        tempD = new Directory(fv,in,tempD->nInode);
      else
        valid=false;
        
      //Check if there is another directory to search for (shown by the "/")
      if(a[0].s[i]=='/')
        i++;
      else
        done=true;
    }
    //Print the contents of this temporary directory, then use the placeholder to change the working directory back to its original location.
    wd = tempD;
    doLsLong((Arg*)1);
    wd = ph;
    
  }
  //Relative path
  else 
  {
    //Create a temporary directory, and a placeholder of the cwd
    Directory* tempD = new Directory(fv,1,0);
    uint pin = wd->iNumberOf((byte *)"..");
    Directory* ph = new Directory(fv,wd->nInode,pin);
    
    //Retrieve the user input and make sure it is a valid relative directory
    byte * lfName = (byte*)arg[0].s;
    uint in = tempD->iNumberOf(lfName);
    if(in)
    {
      //Turn our temporary directory into the requested directory. Temporarily make that our working directory
      tempD = new Directory(fv,in,tempD->nInode);
      wd = tempD;
      
      //Print the contents of this relative directory, then use the placeholder to change the working directory back to its original location.
      doLsLong((Arg*)1);
      wd = ph;
    }
    else
     printf("The requested directory does not exist in the current working directory.\n");    
  }
}




void doInode(Arg * a)
{
  uint ni = a[0].u;

  wd->fv->inodes.show(ni);
}




//Print the inode contents of an entered directory name
void doInodeString(Arg * a)
{
  //Retrieve the inode from the entered string
  uint in = wd->iNumberOf((byte *)a[0].s);
  wd->fv->inodes.show(in);
}




//Create a directory inside of the current directory
void doMkDir(Arg * a)
{

  byte * lfName = (byte *)a[0].s;
  uint in = wd->iNumberOf(lfName);
  
  //Check if the directory we're trying to make already exists in the current directory
  if(in){
    printf("Directory or file with this name already exists.\n");
  }
  else{
    //Get a free inode
    uint inode = fv->inodes.getFree();
    fv->inodes.setLnCnt(inode,1);
    //Create a directory with the entered name and free inode.
    wd->addLeafName(lfName,inode);
    Directory* dir = new Directory(fv,inode,wd->nInode);
    printf("New directory called %s created with inode: %d\n", lfName, inode);
  }
}




//Print the full filepath of the current working directory
void doPwd(Arg * a)
{
  char path [50];
  uint cntr=0;
  
  //Create a temporary directory to traverse our files
  uint in = wd->nInode;
  Directory * dir = new Directory(fv,in,0);
  
  //Traversing toward the root directory
  while(dir->nInode!=1)
  {
    //Saving the inode of the cwd, then changing the cwd to its parent directory
    uint pin = dir->iNumberOf((byte *)"..");
    uint curin = dir->nInode;
    dir = new Directory(fv,pin,0);
    byte * lfName = dir->nameOf(curin);
    
    //Saving the filepath backwards into the path array
    //Also keeping track of the char amount in the filepath
    for (uint i = strlen((char*)lfName)-1; i>=0 && i<50 ; i--) 
    {
      path[cntr] = lfName[i];
      cntr++;
    }
    path[cntr] = '/';
    cntr++;
  }
  
  //If we are not in the root directory, print the stored filepath in reverse.
  if(cntr!=0)
  {
    for(uint i=cntr-1; i>=0 && i<50; i--)
    {
      printf("%c",path[i]);
    }
    printf("\n");
  }
  //If we are in the root directory, print a single "/"
  else
  {
    printf("/\n");
  }
  
}


//Helper method for recursively finding the pathnames in a soft links and changing the cwd to them
uint softCh(char* path)
{
  uint in = 0;
  
  //Absolute path
  if(path[0]=='/')
  {
    //Start from the root directory
    wd = new Directory(fv,1,0);
    
    char loc[50];
    int i=1;
    int j=0;
    bool done = false;
    bool valid = true;
    
    //Check that we are done with the filepath and that each entered directory is a valid one to enter
    while(!done && valid)
    {
    
      //Make sure that we are retrieving one directory at a time
      //Store the input into a character array
      while(path[i]!='/' && ((path[i]<123 && path[i]>96) || (path[i]<91 && path[i]>64)))
      {
        loc[i-1] = path[i];
        i++;
      }
      char* next = (char*)malloc(50);;
      
      //Move the contents of the character array into a pointer
      for(int k=0; j<i-1; k++)
      {
        next[k] += (char)loc[j];
        j++;
      }
      j++;
      next[j]=0;
      
      //Check that the entered directory exists in the cwd
      in = wd->iNumberOf((byte*)next);
      uint type = fv->inodes.getType(in);
      if(in && type == 2)
      {
        Directory* newDir = new Directory(fv,in,wd->nInode);
        wd = newDir;
      }
      //If the file is a soft link, read the pathname stored in the data of this file
      else if(in && type == 3)
      {
        uint size = fv->inodes.getFileSize(in);
        char* nextPth = new char[size]; 
        nextPth = fv->inodes.softRead(in, nextPth);
        uint t = softCh(nextPth);
      }
      else
        valid=false;
      
      //Clear the character pointer for the next directory in the filepath  
      for(int k=0; k<i; k++)
      {
        next[k] = 0;
      }
      
      //Check if there is another directory to look at (indicated by a "/")  
      if(path[i]=='/')
        i++;
      else
        done=true;
    } 
    
  }
  //Relative path
  else if(strcmp(path, ".") != 0)
  {
    //Checks if the desired relative directory is in the cwd
    in = wd->iNumberOf((byte *)path);
    if(in && fv->inodes.getType(in) == 2)
    {
      //Change the working directory to the existing relative directory
      wd = new Directory(fv,in,0);
      
    }
    //If the file is a soft link, read the pathname stored in the data of this file 
    else if(in && fv->inodes.getType(in) == 3)
    {
      uint size = fv->inodes.getFileSize(in);
      char* nextPth = new char[size]; 
      nextPth = fv->inodes.softRead(in, nextPth);
      uint t = softCh(nextPth);
    }
    else
    {
      printf("Target directory does not exist. Staying in current directory.\n");
    }
    
    
  }
  //Return the inode of the cwd
  return in;
  
}



//Change the cwd using either an absolute or relative path
void doChDir(Arg * a)
{
  
  //Absolute path
  if(a[0].s[0]=='/')
  {
    //Start from the root directory
    wd = new Directory(fv,1,0);
    
    char loc[50];
    int i=1;
    int j=0;
    bool done = false;
    bool valid = true;
    
    //Check that we are done with the filepath and that each entered directory is a valid one to enter
    while(!done && valid)
    {
    
      //Make sure that we are retrieving one directory at a time
      //Store the input into a character array
      while(a[0].s[i]!='/' && ((a[0].s[i]<123 && a[0].s[i]>96) || (a[0].s[i]<91 && a[0].s[i]>64)))
      {
        loc[i-1] = a[0].s[i];
        i++;
      }
      char* next = (char*)malloc(50);;
      
      //Move the contents of the character array into a pointer
      for(int k=0; j<i-1; k++)
      {
        next[k] += (char)loc[j];
        j++;
      }
      j++;
      next[j]=0;
      
      //Check that the entered directory exists in the cwd
      uint in = wd->iNumberOf((byte*)next);
      uint type = fv->inodes.getType(in);
      if(in && type == 2)
      {
        Directory* newDir = new Directory(fv,in,wd->nInode);
        wd = newDir;
      }
      //If the file is a soft link, read the pathname stored in the data of this file
      else if(in && type == 3)
      {
        uint size = fv->inodes.getFileSize(in);
        char* path = new char[size]; 
        path = fv->inodes.softRead(in, path);
        
        //This custom function allows this process to be done recursively if needed
        uint t = softCh(path);
      }
      else
        valid=false;
      
      //Clear the character pointer for the next directory in the filepath  
      for(int k=0; k<i; k++)
      {
        next[k] = 0;
      }
      
      //Check if there is another directory to look at (indicated by a "/")  
      if(a[0].s[i]=='/')
        i++;
      else
        done=true;
    }
  }
  //Relative path
  else if(strcmp((char *) a[0].s, ".") != 0)
  {
    //Checks if the desired relative directory is in the cwd
    uint in = wd->iNumberOf((byte *)a[0].s);
    if(in && fv->inodes.getType(in) == 2)
    {
      //Change the working directory to the existing relative directory
      wd = new Directory(fv,in,0);
      
    }
    //If the file is a soft link, read the pathname stored in the data of this file
    else if(in && fv->inodes.getType(in) == 3)
    {
      uint size = fv->inodes.getFileSize(in);
      char* path = new char[size]; 
      path = fv->inodes.softRead(in, path);
      
      //This custom function allows this process to be done recursively if needed
      uint t = softCh(path);
    }
    else
    {
      printf("Target directory does not exist. Staying in current directory.\n");
    }
    
    
  }
  //Print the resulting filepath of the change
  doPwd((Arg*)1);
}



//Remove a subdirectory from the cwd
void doRm(Arg * a)
{
  if(a[0].s[0]=='/')
  {
    uint phIn = wd->nInode;
    
    //Start from the root directory
    wd = new Directory(fv,1,0);
    
    char path1[50];
    int i=1;
    int j=0;
    bool done = false;
    bool valid = true;
    char* file1 = (char*)malloc(50);
    uint in1 = 0;
    char* file2 = (char*)malloc(50);
    uint in2 = 0;
    uint type = 0;
    
    //Check that we are done with the filepath and that each entered directory is a valid one to enter
    while(!done && valid)
    {
      
      //Store the input into a character array
      while(a[0].s[i]!='/' && ((a[0].s[i]<123 && a[0].s[i]>96) || (a[0].s[i]<91 && a[0].s[i]>64)))
      {
        path1[i-1] = a[0].s[i];
        i++;
      }
      char* next = (char*)malloc(50);;
      
      //Move the contents of the character array into a pointer
      for(int k=0; j<i-1; k++)
      {
        next[k] += (char)path1[j];
        j++;
      }
      j++;
      next[j]=0;
      
      //Check that the entered directory exists in the cwd
      //store the values of the first filepath
      uint in = wd->iNumberOf((byte*)next);
      type = fv->inodes.getType(in);
      if(in && type==2)
      {
        Directory* newDir = new Directory(fv,in,wd->nInode);
        wd = newDir;
        file1 = next;
        in1 = in;
      }
      else if(in && type==1)
      {
        file1=next;
        in1=in;
      }
      else if(next[0]!=NULL)
        valid=false;
      
      if(a[0].s[i]=='/')
        i++;
      else
        done=true;
    }
    if(!valid)
    {
      printf("Target file does not exist.\n");
      return;
    }
    
    //Non-soft link deletion
    if(in1 && type != 3)
    {
      //Delete the requested subdirectory
      uint in = wd->deleteFile((byte *) file1, 1);
  
      //Retrieve the number of subdirectories now in the cwd
      uint entNum = wd->entNum()-2;
      uint count = fv->inodes.getLnCnt(in);
      count--;
      fv->inodes.setLnCnt(in,count);
      printf("The removed file is named %s with the inode %d.\nThe current directory now has %d entries.\nThe resulting link count is %d.\n", file1, in, entNum, count);
    }
    //Soft link deletion
    else if(in1 && type == 3)
    {
      //Save current location
      uint pin = wd->iNumberOf((byte *)"..");
      Directory* save = new Directory(fv,pin,0);
      
      //Reading the information of the soft link to appropriately change the link count of the target path
      uint size = fv->inodes.getFileSize(in1);
      char* path = new char[size]; 
      path = fv->inodes.softRead(in1, path);
      uint iPoint = softCh(path);
      uint count = fv->inodes.getLnCnt(iPoint);
      fv->inodes.setLnCnt(iPoint, count-1);
      
      //Return to the saved directory, and delete the soft link
      wd = save;
      wd->deleteFile((byte *) file1, 1);
      
    }
    else
      printf("The directory requested for deletion does not exist in the current working directory.\n");
    
    wd = new Directory(fv,phIn,0);
    
  }
  else
  {
    byte * lfName = (byte *)a[0].s;
    uint in = wd->iNumberOf(lfName);
    uint type = fv->inodes.getType(in);
    
    //Non-soft link deletion
    if(in && type != 3)
    {
      //Delete the requested subdirectory
      uint in = wd->deleteFile((byte *) a[0].s, 1);
  
      //Retrieve the number of subdirectories now in the cwd
      uint entNum = wd->entNum()-2;
      uint count = fv->inodes.getLnCnt(in);
      count--;
      fv->inodes.setLnCnt(in,count);
      printf("The removed directory is named %s with the inode %d.\nThe current directory now has %d entries.\nThe resulting link count is %d.\n", a[0].s, in, entNum, count);
    }
    //Soft link deletion
    else if(in && type == 3)
    {
      //Save current location
      uint pin = wd->iNumberOf((byte *)"..");
      Directory* save = new Directory(fv,pin,0);
      
      //Reading the information of the soft link to appropriately change the link count of the target path
      uint size = fv->inodes.getFileSize(in);
      char* path = new char[size]; 
      path = fv->inodes.softRead(in, path);
      uint iPoint = softCh(path);
      uint count = fv->inodes.getLnCnt(iPoint);
      fv->inodes.setLnCnt(iPoint, count-1);
      
      //Return to the saved directory, and delete the soft link
      wd = save;
      wd->deleteFile((byte *) a[0].s, 1);
    }
    else
      printf("The directory requested for deletion does not exist in the current working directory.\n");
  
  }

}


//Recursively remove directories/subdirectories
void rmRec(Arg * a)
{
  byte * lfName = (byte *)a[0].s;
  uint in = wd->iNumberOf(lfName);
  
  //Check that the directory we want to delete exists in the cwd
  if(in)
  {
    uint inode =0;
    while(wd->entNum()>2)
    {
      inode = wd->next();
    }
    //Delete the requested subdirectory
    uint in = wd->deleteFile((byte *) a[0].s, 1);
  
    //Retrieve the number of subdirectories now in the cwd
    uint entNum = wd->entNum()-2;
  
    printf("The removed directory named %s with the inode %d, and %d entries.\n", a[0].s, in, entNum);
  }
  else
    printf("The directory requested for deletion does not exist in the current working directory.\n");
}





//Move one file to another file location
void doMv(Arg * a)
{
  //Absolute paths
  if(a[0].s[0]=='/' && a[1].s[0]=='/')
  {
    uint phIn = wd->nInode;
    
    //Start from the root directory
    wd = new Directory(fv,1,0);
    
    char path1[50];
    int i=1;
    int j=0;
    bool done = false;
    bool valid = true;
    char* file1 = (char*)malloc(50);
    uint in1 = 0;
    char* file2 = (char*)malloc(50);
    uint in2 = 0;
    uint type = 0;
    
    //Check that we are done with the filepath and that each entered directory is a valid one to enter
    while(!done && valid)
    {
    
      //Make sure that we are retrieving one directory at a time
      //Store the input into a character array
      while(a[0].s[i]!='/' && ((a[0].s[i]<123 && a[0].s[i]>96) || (a[0].s[i]<91 && a[0].s[i]>64)))
      {
        path1[i-1] = a[0].s[i];
        i++;
      }
      char* next = (char*)malloc(50);;
      
      //Move the contents of the character array into a pointer
      for(int k=0; j<i-1; k++)
      {
        next[k] += (char)path1[j];
        j++;
      }
      j++;
      next[j]=0;
      
      //Check that the entered directory exists in the cwd
      //store the values of the first filepath
      uint in = wd->iNumberOf((byte*)next);
      type = fv->inodes.getType(in);
      if(in && type==2)
      {
        Directory* newDir = new Directory(fv,in,wd->nInode);
        wd = newDir;
        file1 = next;
        in1 = in;
      }
      else if(type!=1)
        valid=false;
      else
      {
        file1 = next;
        in1 = in;
      }
      
      if(a[0].s[i]=='/')
        i++;
      else
        done=true;
    }
    //Go into the parent of the cwd and delete the cwd
    if(valid && type==2)
    {
      uint pin = wd->iNumberOf((byte *)"..");
      Directory* par = new Directory(fv,pin,0);
      wd = par;
      wd->deleteFile((byte*)file1,0);
    }
    else if(type ==1)
      wd->deleteFile((byte*)file1,0);
    //If the first file does not exist, return nothing
    else
    {
      printf("The directory you want to move does not exist.\n");
      return;
    }
    
    //Reset the variables
    wd = new Directory(fv,1,0);
    valid = true;
    done = false;
    i=1;
    j=0;
    uint backup=1;
    
    //Check that we are done with the filepath and that each entered directory is a valid one to enter
    while(!done && valid)
    {
    
      //Make sure that we are retrieving one directory at a time
      //Store the input into a character array
      while(a[1].s[i]!='/' && ((a[1].s[i]<123 && a[1].s[i]>96) || (a[1].s[i]<91 && a[1].s[i]>64)))
      {
        path1[i-1] = a[1].s[i];
        i++;
      }
      char* next = (char*)malloc(50);;
      
      //Move the contents of the character array into a pointer
      for(int k=0; j<i-1; k++)
      {
        next[k] += (char)path1[j];
        j++;
      }
      j++;
      next[j]=0;
      
      //Check that the entered directory exists in the cwd
      //Store the values of the second filepath
      uint in = wd->iNumberOf((byte*)next);
      if(in)
      {
        Directory* newDir = new Directory(fv,in,wd->nInode);
        wd = newDir;
        file2 = next;
        in2 = in;
        backup = in;
      }
      else
      {
        valid=false;
        file2 = next;
        in2 = backup;
      }
      
      if(a[1].s[i]=='/')
        i++;
      else
        done=true;
    }
    
    //Place the first filepath's file into the second filepath's location
    if(valid)
    {
      wd = new Directory(fv,in2,wd->nInode);
      wd->addLeafName((byte*)file1,in1);
      Directory* ph = new Directory(fv,in1,0);
    
      wd = new Directory(fv,phIn,0);
    }
    //Rename the first filepath to the second filepath's location's name
    else
    {
      wd = new Directory(fv,in2,wd->nInode);
      uint pin = wd->nInode;
      wd->addLeafName((byte*)file2,in1);
      Directory* ph = new Directory(fv,in1,pin);
    }
    
  }
  //Relative paths
  else if(a[0].s[1]!='/' && a[1].s[1]!='/')
  {
    //Get the names and inodes of both files/directories
    byte * dirToMv = (byte *)a[0].s;
    byte * tarDir = (byte *)a[1].s;
    uint subIn = wd->iNumberOf(dirToMv);
    uint tarIn = wd->iNumberOf(tarDir);
  
    //If the moving directory/file doesn't exist, do nothing
    if(!subIn)
      printf("The directory you want to move does not exist.\n");
    //If the target directory doesn't exist, change the name of the moving directory to that of the target directory
    else if(!tarIn)
    {
      wd->deleteFile(dirToMv,0);
    
      uint pin = wd->nInode;
      wd->addLeafName(tarDir,subIn);
      Directory* ph = new Directory(fv,subIn,pin);
    
    }
    //If both files exist   
    else
    {
      //Make sure that we aren't trying to put files into non-directories
      if(fv->inodes.getType(tarIn) == 1)
        printf("You cannot put a file/directory into a regular file.\n");
      else
      {
        wd->deleteFile(dirToMv,0);
      
        //Save cwd info to set back after the move
        uint prevPin = wd->iNumberOf((byte *)"..");
        uint pin = wd->nInode;
      
        //Move into the target directory and place the moving directory
        wd = new Directory(fv,tarIn,wd->nInode);
        wd->addLeafName(dirToMv,subIn);
        Directory* ph = new Directory(fv,subIn,0);
        wd = new Directory(fv,pin,prevPin);
      }
    }
  }
}



void doHardLn(Arg * a)
{
  //Relative
  if(a[0].s[0]!='/')
  {
    byte * linked = (byte *)a[0].s;
    byte * newLn = (byte *)a[1].s;
    uint in = wd->iNumberOf(linked);
    uint inode = wd->iNumberOf(newLn);
    uint tarType = fv->inodes.getType(in);
  
    //
    if(!in){
      printf("Target file does not exist.\n");
    }
    else if(tarType==2){
      printf("Target cannot be a directory.\n");
    }
    else if(inode){
      printf("The link file already exists.\n");
    }
    else{
      uint count = fv->inodes.getLnCnt(in);
      count++;
      fv->inodes.setLnCnt(in,count);
      wd->addLeafName(newLn,in);
      printf("New hard link called %s (linked to %s) created with inode: %d\n", newLn, linked, in);
      }
  }
  //Absolute
  else if(a[0].s[0]=='/' && a[1].s[0]=='/' || a[0].s[0]=='/' && a[1].s[0]=='.')
  {
    uint phIn = wd->nInode;
    
    //Start from the root directory
    wd = new Directory(fv,1,0);
    
    char path1[50];
    int i=1;
    int j=0;
    bool done = false;
    bool valid = true;
    char* file1 = (char*)malloc(50);
    uint in1 = 0;
    char* file2 = (char*)malloc(50);
    uint in2 = 0;
    uint type = 0;
    
    //Check that we are done with the filepath and that each entered directory is a valid one to enter
    while(!done && valid)
    {
      
      //Store the input into a character array
      while(a[0].s[i]!='/' && ((a[0].s[i]<123 && a[0].s[i]>96) || (a[0].s[i]<91 && a[0].s[i]>64)))
      {
        path1[i-1] = a[0].s[i];
        i++;
      }
      char* next = (char*)malloc(50);;
      
      //Move the contents of the character array into a pointer
      for(int k=0; j<i-1; k++)
      {
        next[k] += (char)path1[j];
        j++;
      }
      j++;
      next[j]=0;
      
      //Check that the entered directory exists in the cwd
      //store the values of the first filepath
      uint in = wd->iNumberOf((byte*)next);
      type = fv->inodes.getType(in);
      if(in && type==2)
      {
        Directory* newDir = new Directory(fv,in,wd->nInode);
        wd = newDir;
        file1 = next;
        in1 = in;
      }
      else if(in && type==1)
      {
        file1=next;
        in1=in;
      }
      else if(next[0]!=NULL)
        valid=false;
      
      if(a[0].s[i]=='/')
        i++;
      else
        done=true;
    }
    //Go into the parent of the cwd and delete the cwd
    if(type==2)
    {
      printf("Target cannot be a directory.\n");
      return;
    }
    else if(!valid)
    {
      printf("Target file does not exist.\n");
      return;
    }
    
    
    if(a[1].s[0]!='.')
    {
      //Reset the variables
      wd = new Directory(fv,1,0);
      valid = true;
      done = false;
      bool ordFile = false;
      i=1;
      j=0;
      uint backup=1;
    
      //Check that we are done with the filepath and that each entered directory is a valid one to enter
      while(!done && valid)
      {
    
        //Make sure that we are retrieving one directory at a time
        //Store the input into a character array
        while(a[1].s[i]!='/' && ((a[1].s[i]<123 && a[1].s[i]>96) || (a[1].s[i]<91 && a[1].s[i]>64)))
        {
          path1[i-1] = a[1].s[i];
          i++;
        }
        char* next = (char*)malloc(50);;
      
        //Move the contents of the character array into a pointer
        for(int k=0; j<i-1; k++)
        {
          next[k] += (char)path1[j];
          j++;
        }
        j++;
        next[j]=0;
      
        //Check that the entered directory exists in the cwd
        //Store the values of the second filepath
        uint in = wd->iNumberOf((byte*)next);
        uint type = fv->inodes.getType(in);
        if(in && type==2)
        {
          Directory* newDir = new Directory(fv,in,wd->nInode);
          wd = newDir;
          file2 = next;
          in2 = in;
          backup = in;
        }
        else if(in && type==1)
        {
          printf("The link file already exists.\n");
        }
        else if(next[0]!=NULL)
        {
          file2 = next;
          ordFile = true;
        }
      
        if(a[1].s[i]=='/' && !ordFile)
          i++;
        else
          done=true;
      }
    
      //
      if(valid)
      {
        //Increase the link count
        uint count = fv->inodes.getLnCnt(in1);
        count++;
        fv->inodes.setLnCnt(in1,count);
        
        //Create the new hard link
        wd->addLeafName((byte*)file2,in1);
        printf("New hard link called %s (linked to %s) created with inode: %d\n", (char*)file2, (char*)file1, in1);
      }
    
      wd = new Directory(fv,phIn,0);
    }
    else
    {
      //Increase the link count
      wd = new Directory(fv,phIn,0);
      uint count = fv->inodes.getLnCnt(in1);
      count++;
      fv->inodes.setLnCnt(in1,count);
      
      //Create the local hard link
      wd->addLeafName((byte*)file1,in1);
      printf("New hard link called %s (linked to %s) created with inode: %d\n", (char*)file1, (char*)file1, in1);
      
    }
    
  }
  
}


//The same as the regular hard link (if a dot is used), but without the necessity of a dot.
void doHardLnNoDot(Arg * a)
{ 
  uint phIn = wd->nInode;
    
  //Start from the root directory
  wd = new Directory(fv,1,0);
    
  char path1[50];
  int i=1;
  int j=0;
  bool done = false;
  bool valid = true;
  char* file1 = (char*)malloc(50);
  uint in1 = 0;
  uint in2 = 0;
  uint type = 0;
    
  //Check that we are done with the filepath and that each entered directory is a valid one to enter
  while(!done && valid)
  {
      
    //Store the input into a character array
    while(a[0].s[i]!='/' && ((a[0].s[i]<123 && a[0].s[i]>96) || (a[0].s[i]<91 && a[0].s[i]>64)))
    {
      path1[i-1] = a[0].s[i];
      i++;
    }
    char* next = (char*)malloc(50);;
      
    //Move the contents of the character array into a pointer
    for(int k=0; j<i-1; k++)
    {
      next[k] += (char)path1[j];
      j++;
    }
    j++;
    next[j]=0;
      
    //Check that the entered directory exists in the cwd
    //store the values of the first filepath
    uint in = wd->iNumberOf((byte*)next);
    type = fv->inodes.getType(in);
    if(in && type==2)
    {
      Directory* newDir = new Directory(fv,in,wd->nInode);
      wd = newDir;
      file1 = next;
      in1 = in;
    }
    else if(in && type==1)
    {
      file1=next;
      in1=in;
    }
    else if(next[0]!=NULL)
      valid=false;
      
    if(a[0].s[i]=='/')
      i++;
    else
      done=true;
  }
  //Go into the parent of the cwd and delete the cwd
  if(type==2)
  {
    printf("Target cannot be a directory.\n");
    return;
  }
  else if(!valid)
  {
    printf("Target file does not exist.\n");
    return;
  }
  
  //Increase the link count
  wd = new Directory(fv,phIn,0);
  uint count = fv->inodes.getLnCnt(in1);
  count++;
  fv->inodes.setLnCnt(in1,count);
  
  //Create the hard link
  wd->addLeafName((byte*)file1,in1);
  printf("New hard link called %s (linked to %s) created with inode: %d\n", (char*)file1, (char*)file1, in1);
}



void doSoftLn(Arg * a)
{
  if(a[0].s[0]!='-' || a[0].s[1]!='s')
  {
    printf("The only optional ln command is -s\n");
    return;
  }
  
  //Relative
  if(a[1].s[0]!='/')
  {
    byte * linked = (byte *)a[1].s;
    byte * newLn = (byte *)a[2].s;
    uint in = wd->iNumberOf(linked);
    uint inode = fv->inodes.getFree();
    uint tarType = fv->inodes.getType(in);
  
    //
    if(!in){
      printf("Target file does not exist.\n");
    }
    else{
      //Increase the link count of the target
      uint count = fv->inodes.getLnCnt(in);
      count++;
      fv->inodes.setLnCnt(in,count);
      
      //Create the soft link and set link count to 1
      wd->addLeafName(newLn,inode);
      fv->inodes.setLnCnt(inode,1);
      fv->inodes.setType(inode,3);
      
      char* str = new char[256];
      str = (char*)linked;
      
      //Writing the target path name to the data of the soft link file
      fv->inodes.softWrite(inode, str);
      printf("New soft link called %s (linked to %s) created with inode: %d\n", newLn, linked, inode);
      }
  }
  //Absolute
  else if(a[1].s[0]=='/')
  {
    uint phIn = wd->nInode;
    
    //Start from the root directory
    wd = new Directory(fv,1,0);
    
    char path1[50];
    int i=1;
    int j=0;
    bool done = false;
    bool valid = true;
    char* file1 = (char*)malloc(50);
    char* fullPth = (char*)malloc(50);
    uint index = 0;
    uint in1 = 0;
    char* file2 = (char*)malloc(50);
    uint in2 = 0;
    uint type = 0;
    
    //Check that we are done with the filepath and that each entered directory is a valid one to enter
    while(!done && valid)
    {
      
      //Store the input into a character array
      while(a[1].s[i]!='/' && ((a[1].s[i]<123 && a[1].s[i]>96) || (a[1].s[i]<91 && a[1].s[i]>64)))
      {
        path1[i-1] = a[1].s[i];
        i++;
      }
      char* next = (char*)malloc(50);;
      
      //Move the contents of the character array into a pointer
      for(int k=0; j<i-1; k++)
      {
        next[k] += (char)path1[j];
        j++;
      }
      j++;
      next[j]=0;
      
      fullPth[index] = '/';
      index++;
      for(uint k=0; k<j-1; k++)
      {
        fullPth[index] = next[k];
        index++;
      }
      
      //Check that the entered directory exists in the cwd
      //store the values of the first filepath
      uint in = wd->iNumberOf((byte*)next);
      type = fv->inodes.getType(in);
      if(in && type==2)
      {
        Directory* newDir = new Directory(fv,in,wd->nInode);
        wd = newDir;
        file1 = next;
        in1 = in;
      }
      else if(in && type==1)
      {
        file1=next;
        in1=in;
      }
      else if(next[0]!=NULL)
        valid=false;
      
      if(a[1].s[i]=='/')
        i++;
      else
        done=true;
    }
    //Go into the parent of the cwd and delete the cwd
    if(!valid)
    {
      printf("Target file does not exist.\n");
      return;
    }
    
    
    //Reset the variables
    wd = new Directory(fv,1,0);
    valid = true;
    done = false;
    bool ordFile = false;
    i=1;
    j=0;
    uint backup=1;
    
    //Check that we are done with the filepath and that each entered directory is a valid one to enter
    while(!done && valid)
    {
    
      //Make sure that we are retrieving one directory at a time
      //Store the input into a character array
      while(a[2].s[i]!='/' && ((a[2].s[i]<123 && a[2].s[i]>96) || (a[2].s[i]<91 && a[2].s[i]>64)))
      {
        path1[i-1] = a[2].s[i];
        i++;
      }
      char* next = (char*)malloc(50);;
      
      //Move the contents of the character array into a pointer
      for(int k=0; j<i-1; k++)
      {
        next[k] += (char)path1[j];
        j++;
      }
      j++;
      next[j]=0;
      
      //Check that the entered directory exists in the cwd
      //Store the values of the second filepath
      uint in = wd->iNumberOf((byte*)next);
      uint type = fv->inodes.getType(in);
      if(in && type==2)
      {
        Directory* newDir = new Directory(fv,in,wd->nInode);
        wd = newDir;
        file2 = next;
        in2 = in;
        backup = in;
      }
      else if(in && type==1)
      {
        printf("The link file already exists.\n");
      }
      else if(next[0]!=NULL)
      {
        file2 = next;
        ordFile = true;
        in2 = 0;
      }
      
      if(a[2].s[i]=='/' && !ordFile)
        i++;
      else
        done=true;
    }
    
    
    if(valid)
    {
      if(in2==0)
        in2 = fv->inodes.getFree();
      
      //Increase link count of target path
      uint count = fv->inodes.getLnCnt(in1);
      count++;
      fv->inodes.setLnCnt(in1,count);
      
      //Create a soft link and set link count to 1
      wd->addLeafName((byte*)file2,in2);
      fv->inodes.setLnCnt(in2,1);
      fv->inodes.setType(in2,3);
      
      char* str = new char[256];
      str = (char*)fullPth;
      
      //Writing the target path name to the data of the soft link file
      fv->inodes.softWrite(in2, str);
      printf("New soft link called %s (linked to %s) created with inode: %d\n", file2, fullPth, in2);
    }
    
    wd = new Directory(fv,phIn,0);
    
  }
}

void doMountDF(Arg * a)		// arg a ignored
{
  TODO("doMountDF");
}

void doMountUS(Arg * a)
{
  TODO("doMountUS");
}

void doUmount(Arg * a)
{
  TODO("doUmount");
}

/* The following describes one entry in our table of commands.  For
 * each cmmdName (a null terminated string), we specify the arguments
 * it requires by a sequence of letters.  The letter s stands for
 * "that argument should be a string", the letter u stands for "that
 * argument should be an unsigned int."  The data member (func) is a
 * pointer to the function in our code that implements that command.
 * globalsNeeded identifies whether we need a volume ("v"), a simdisk
 * ("d"), or a mount table ("m").  See invokeCmd() below for exact
 * details of how all these flags are interpreted.
 */

class CmdTable {
public:
  char *cmdName;
  char *argsRequired;
  char *globalsNeeded;		// need d==simDisk, v==cfv, m=mtab
  void (*func) (Arg * a);
} cmdTable[] = {
  {"cd", "s", "v", doChDir},
  {"cp", "ss", "v", doCopy},
  {"echo", "ssss", "", doEcho},
  {"inode", "u", "v", doInode},
  {"inode", "s", "v", doInodeString},
  {"ls", "", "v", doLsLong},
  {"ln", "ss", "", doHardLn},
  {"ln", "s", "", doHardLnNoDot},
  {"ln", "sss", "", doSoftLn},
  {"ls", "s", "v", doLsofDir},
  {"lslong", "", "v", doLsLong},
  {"mkdir", "s", "v", doMkDir},
  {"mkdisk", "s", "", doMakeDisk},
  {"mkfs", "s", "", doMakeFV},
  {"mount", "us","", doMountUS},
  {"mount", "", "", doMountDF},
  {"mv", "ss", "v", doMv},
  {"rddisk", "su", "", doReadDisk},
  {"rmdir -r", "s", "v", rmRec},
  {"rmdir", "s", "v", doRm},
  {"rm -r", "s", "v", rmRec},
  {"rm", "s", "v", doRm},
  {"pwd", "", "v", doPwd},
  {"q", "", "", doQuit},
  {"quit", "", "", doQuit},
  {"umount", "u", "m", doUmount},
  {"wrdisk", "sus", "", doWriteDisk}
};

uint ncmds = sizeof(cmdTable) / sizeof(CmdTable);

void usage()
{
  printf("The shell has only the following cmds:\n");
  for (uint i = 0; i < ncmds; i++)
    printf("\t%s\t%s\n", cmdTable[i].cmdName, cmdTable[i].argsRequired);
  printf("Start with ! to invoke a Unix shell cmd\n");
}

/* pre:: k >= 0, arg[] are set already;; post:: Check that args are
 * ok, and the needed simDisk or cfv exists before invoking the
 * appropriate action. */

void invokeCmd(int k, Arg *arg)
{
  uint ok = 1;
  if (cmdTable[k].globalsNeeded[0] == 'v' && cwdVNIN == 0) {
    ok = 0;
    printf("Cmd %s needs the cfv to be != 0.\n", cmdTable[k].cmdName);
  }
  else if (cmdTable[k].globalsNeeded[0] == 'm' && mtab == 0) {
    ok = 0;
    printf("Cmd %s needs the mtab to be != 0.\n", cmdTable[k].cmdName);
  }

  char *req = cmdTable[k].argsRequired;
  uint na = strlen(req);
  for (uint i = 0; i < na; i++) {
    if (req[i] == 's' && (arg[i].s == 0 || arg[i].s[0] == 0)) {
      ok = 0;
      printf("arg #%d must be a non-empty string.\n", i);
    }
    if ((req[i] == 'u') && (arg[i].s == 0 || !isDigit(arg[i].s[0]))) {
	ok = 0;
	printf("arg #%d (%s) must be a number.\n", i, arg[i].s);
    }
  }
  if (ok)
    (*cmdTable[k].func) (arg);
}

/* pre:: buf[] is the command line as typed by the user, nMax + 1 ==
 * sizeof(types);; post:: Parse the line, and set types[], arg[].s and
 * arg[].u fields.
 */

void setArgsGiven(char *buf, Arg *arg, char *types, uint nMax)
{
  for (uint i = 0; i < nMax; i++) {
    arg[i].s = 0;
    types[i] = 0;
  }
  types[nMax] = 0;

  strtok(buf, " \t\n");		// terminates the cmd name with a \0

  for (uint i = 0; i < nMax;) {
      char *q = strtok(0, " \t");
      if (q == 0 || *q == 0) break;
      arg[i].s = q;
      arg[i].u = toNum(q);
      types[i] = isDigit(*q)? 'u' : 's';
      nArgs = ++i;
  }
}

/* pre:: name pts to the command token, argtypes[] is a string of
 * 's'/'u' indicating the types of arguments the user gave;; post::
 * Find the row number of the (possibly overloaded) cmd given in
 * name[].  Return this number if found; return -1 otherwise. */

int findCmd(char *name, char *argtypes)
{
  for (uint i = 0; i < ncmds; i++) {
    if (strcmp(name, cmdTable[i].cmdName) == 0
	&& strcmp(argtypes, cmdTable[i].argsRequired) == 0) {
      return i;
    }
  }
  return -1;
}

void ourgets(char *buf) {
  fgets(buf, 1024, stdin);
  char * p = index(buf, '\n');
  if (p) *p = 0;
}

int main()
{
  char buf[1024];		// better not type longer than 1023 chars

  usage();
  for (;;) {
    *buf = 0;			// clear old input
    printf("%s", "sh33% ");	// prompt
    ourgets(buf);
    printf("cmd [%s]\n", buf);	// just print out what we got as-is
    if (buf[0] == 0)
      continue;
    if (buf[0] == '#')
      continue;			// this is a comment line, do nothing
    if (buf[0] == '!')		// begins with !, execute it as
      system(buf + 1);		// a normal shell cmd
    else {
      setArgsGiven(buf, arg, types, nArgsMax);
      int k = findCmd(buf, types);
      if (k >= 0)
	invokeCmd(k, arg);
      else
	usage();
    }
  }
}

// -eof-

