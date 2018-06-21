$! make xmaddressbook under VMS
$!
$! You will need the Xvmsutils-lib to build and link. At least version 1.07
$! is requiered. You may get the package from 
$! http://axp616.gsi.de:8080/www/vms/sw/xvmsutils.htmlx
$! 
$! Also MMS or MMK are requiered to use the descrip.mms. 
$! MMS you may buy from DEC, MMK is available from
$! http://axp616.gsi.de:8080/www/vms/sw/mmk.htmlx
$! If you have problems with the build you may contact me at 
$! M.Zinser@gsi.de. 
$!
$! Look for MMS/MMK
$ if f$search("SYS$SYSTEM:MMS.EXE").eqs.""
$   then
$    make = "mmk"
$   else
$    make = "mms"
$ endif
$!
$! Look for the compiler used
$!
$ if f$getsyi("HW_MODEL").ge.1024
$ then
$  comp  = "__decc__=1"
$  if f$trnlnm("SYS").eqs."" then define sys sys$library:
$ else
$  if f$search("SYS$SYSTEM:DECC$COMPILER.EXE").eqs.""
$   then
$    comp  = "__vaxc__=1"
$    if f$trnlnm("SYS").eqs."" then define sys sys$library:
$   else
$    if f$trnlnm("SYS").eqs."" then define sys decc$library_include:
$    comp  = "__decc__=1"
$  endif
$ endif
$!
$!      Build the option-file
$!
$ open/write optf xmaddressbook.opt
$ write optf "Identification=""Xmaddress 1.5.1"""
$ write optf "x11vms:xvmsutils.olb/lib"
$!
$!
$!  Find out which X-Version we're running.  This will fail for older
$!  VMS versions (i.e., v5.5-1).  Therefore, choose DECWindows XUI for
$!  default.
$!
$ On Error Then GoTo XUI
$ @sys$update:decw$get_image_version sys$share:decw$xlibshr.exe decw$version
$ if f$extract(4,3,decw$version).eqs."1.0"
$ then
$   write optf "Sys$share:DECW$DWTLIBSHR.EXE/Share"
$ endif
$ if f$extract(4,3,decw$version).eqs."1.1"
$ then
$   write optf "sys$share:decw$xmlibshr.exe/share"
$   write optf "sys$share:decw$xtshr.exe/share"
$ endif
$ if f$extract(4,3,decw$version).eqs."1.2"
$ then
$   write optf "sys$share:decw$xmlibshr12.exe/share"
$   write optf "sys$share:decw$xtlibshrr5.exe/share"
$ endif
$ GoTo MAIN
$!
$XUI:
$!
$   write optf "Sys$share:DECW$DWTLIBSHR.EXE/Share"
$MAIN:
$!
$ write optf "sys$share:decw$xlibshr.exe/share"
$ close optf
$!
$! Build the thing
$!
$ write sys$output "Compiling Xmaddressbook sources ..."
$   'make'/macro=('comp')
$ write sys$output "Xmaddressbook build completed"
$ exit
