# This program converts a vCard to the Xmaddressbook format. 

BEGIN {FS=":"; print "#RS1.1"} 

/^BEGIN:VCARD/ {entry++}; 

/^N/ { 
    nfields=split($2,ndata,";");
    firstname=ndata[2]; 
    lastname=ndata[1];
    midname=ndata[3]; 
    npref=ndata[4];
    nsuff=ndata[5]; 
} 
/^FN/ {fullname=$2}
/^ORG/ {organization=$2}
/^URL/ {url=$2} 
# Addresses are formatted as: 
# ;Suite 101;38777 West Six Mile Road;Livonia;MI;48152;USA
#PO Box;Ext. Address ; Street                 ;Town; State; ZIP; Country 
# TYPE can be DOM INTL POSTAL PARCEL HOME or WORK 
/^ADR/ { naddr=1; 
    if ($1~"HOME") {naddr=2}; 
    if ($1~"WORK") {naddr=1};
    if ($1~"DOM") {country[naddr]="France"};
    afields=split($2,adata,";");
    pobox[naddr]=adata[1]; 
    extaddress[naddr]=adata[2]; 
    street[naddr]=adata[3]; 
    town[naddr]=adata[4]; 
    state[naddr] = adata[5];
    zip[naddr] = adata[6]; 
    country[naddr]=adata[7]; 
	
} 
# Telephone number carry: (see vCard specification p. 14) 
# TEL;TYPE=WORK,MSG (Voice) 
# TEL;TYPE=WORK,FAX:(Fax) 
/^TEL/ {
    if (($1~"WORK") && ($1!~"FAX")) {tel[1]=$2}; 
    if (($1~"HOME") && ($1!~"FAX")) {tel[2]=$2};
    if ($1~"CELL") {cell=$2}; 
    if ($1~"FAX") {fax=$2};
} 
/^EMAIL/ {email=$2} 
/^END:VCARD/ {
    if (fullname!="") {
	print fullname 
    } else {
	printf ("%s %s %s %s, %s\n",npref,firstname,lastname,midname,nsuff)
    }
    printf ("%s\n%s\n",tel[1],tel[2])
    printf ("%s\n",organization);
    for (i=1;i<=2;i++) {
	printf ("%s %s %s %s %s %s %s\n",pobox[i],extaddress[i],street[i],town[i],state[i],zip[i],country[i]);	
    }
    printf ("%s\n",email); 
    printf ("%s\n",fax); 
    printf ("%s\n",strftime()); 
    printf ("Web: %s\nCell: %s\n #RE\n",url,cell); 
    for (i=1;i<=2;i++) {
    tel[i]=""
    pobox[i]=""
    extaddress[i]=""
    street[i]=""
    town[i]=""
    state[i] = ""
    zip[i] = ""
    country[i]=""
    } 
    birthday=""
    firstname=""
    lastname=""
    midname=""
    npref=""
    nsuff=""
    fax=""
    cell="" 
    fullname=""
    organization=""
}

 

# Names are formatted as: 
# Last Name; First Name; Middle Names; Name prefix (ex: Mr./Ms./Mrs./Dr./Pr. etc...); Name Suffix (ex: Esq., Jr., MD, etc...) 


# EMAIL can be INTERNET (default); TLX (Telex) ; X400 (Norme CCITT X.400) 
# GEO: indicates longitude;latitude 
# TITLE: indique un titre 
# 