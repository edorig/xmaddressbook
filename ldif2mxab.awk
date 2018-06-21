BEGIN {FS=":"; print "#RS1.1"}

/^sn:/ {lname=$2}
/^givenName:/ {fname=$2} 
/^cn:/ {dname=$2} 
/^mail:/ {email=$2} 
/^homePhone:/ {hphone=$2}
/^mozillaHomeStreet:/ {home2=$2; if (home2=="") {cmd ="echo" $3 "|base64 -di |recode u8..l1"; cmd | getline home2; close(cmd) }} 
/^mozillaHomeLocalityName:/ {hcity=$2; if (hcity=="") {cmd ="echo" $3 "|base64 -di |recode u8..l1"; cmd | getline hcity; close(cmd) } } 
/^mozillaHomePostalCode:/ {hzip=$2} 
/^mozillaHomeCountryName:/ {hcountry=$2}
/^mail:/ {email1=$2} 
/^telephoneNumber:/ {wphone=$2}
/^street:/ {work2=$2}
/^l:/ {wcity=$2} 
/^o:/ {org=$2}
/^ou:/{dept=$2} 
/^mozillaHomeUrl:/ {www1=$2":"$3} 


/^$/ {
if (dname=="") {dname=lname" "fname}; 
	printf("%s\n",dname);
	printf("%s\n",wphone);
	printf("%s\n",hphone);
	printf("%s %s\n",dept,org);
	printf("%s %s %s %s %s %s\n",work,work2,wzip,wcity,wstate,wcountry);
	printf("%s %s %s %s %s %s\n",home,home2,hzip,hcity,hstate,hcountry);
	printf("%s\n",email1);
	printf("%s\n",fax);
	printf("%s\n",strftime());
	printf ("Pager: %s\n",pager); 
	printf ("Cell Phone: %s\n",cell); 
	printf ("Skype : %s\n",sname); 
	printf ("Web: %s %s \n",www1,www2); 
	print "#RE";
	dname=""
	lname=""
	fname=""
	wphone=""
	hphone=""
	dept=""
	org=""
	work=""
	work2=""
	wcity=""
	wstate=""
	wzip=""
	wcountry=""
	home=""
	home2=""
	hcity=""
	hstate=""
	hzip=""
	hcountry=""
	email1=""
	fax=""
	pager=""
	cell=""
	sname=""
	www1=""
	www2=""
}


    
