#include <stdio.h>
#include <mntent.h>
#include <fstab.h>
#include <sys/statvfs.h>
void printvfstat(struct statvfs *fsd){
   printf("File system block size: %d\n"
         "Fundamental file system block size (fragment size): %d\n"
         "Total number of blocks on the file system, in units of f_frsize: %d\n"
         "Total number of free blocks.%d\n"
         "Total number of free blocks available to non-privileged processes: %d\n"
         "Total number of file nodes (inodes) on the file system: %d\n"
         "Total number of free file nodes (inodes): %d\n"
         "Total number of free file nodes (inodes) available to non-privileged processes: %d\n"
         "File system ID number: %d\n"
         "Flags, described in the Flags section: %d\n"
         "Maximum length of a file name (path element): %ld\n",
         fsd->f_bsize,
         fsd->f_frsize,
         fsd->f_blocks,
         fsd->f_bfree,
         fsd->f_bavail,
         fsd->f_files,
         fsd->f_ffree,
         fsd->f_favail,
         fsd->f_fsid,
         fsd->f_flag,
         fsd->f_namemax);
}

int main(){
   struct mntent *mnt;
   FILE *fp;
   fp = setmntent(_PATH_MOUNTED, "r");
   if(!fp)
      return 0;

   while((mnt=getmntent(fp))){
      printf("%s\t", mnt->mnt_fsname);
      printf("%s\t", mnt->mnt_dir);
      printf("%s\t", mnt->mnt_type);
      printf("%s\t", mnt->mnt_opts);
      printf("%d\t", mnt->mnt_freq);
      printf("%d\n", mnt->mnt_passno);
      printf("-------------------------\n");
      struct statvfs fsd;
      char *stat_file;
      if(statvfs(mnt->mnt_dir, &fsd)>=0){
         printvfstat(&fsd);
         printf("************************************\n");
      }
   }
   endmntent(fp);
}
