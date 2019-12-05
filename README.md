# findUtil
a command line find utility for linux. 

./find 
  prints the contents of your current working directory

./find <whereToLook>
  prints the contents of the specified directory <whereToLook>
  
./find <whereToLook> -name <name>
  finds and prints path of the file whose name is given
  -n also works for this
  
./find <whereToLook> -inum <inum> 
    prints all the files in the specified directory who has an inum of <inum>
  e.g. ./find testDir -inum 5556767
  output:
    ./testDir/file1
  -i also works for inum search
  
./find <whereToLook> -mmin <min> 
   prints all contents who have been modified within the <min> value
  e.g ./find testDir -mmin 10
      prints all files who have been modified later than 10 minutes ago
  e.g. ./find testDir -mmin -10
      prints all files who have been modified lass than 10 minutes ago
  
 ./find <whereToLook> -criteria <argument> -delete
    takes any search criteria and deletes what matches the criteria
    e.g. ./find testDir -name file1 -delete
        deletes any file in testDir that is named file1
