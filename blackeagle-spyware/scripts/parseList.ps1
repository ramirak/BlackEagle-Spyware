foreach($line in Get-Content temp/netlog | Get-Unique) {
    if($line -match $regex){
         Retrieve-ServerCertFromSocket $line 443 | fl subject,*not*,Thumb*,ser* | findstr "Subject"
    }
}