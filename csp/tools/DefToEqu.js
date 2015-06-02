var fs = WScript.CreateObject("Scripting.FileSystemObject");

args = WScript.Arguments;

if(args.length >= 2)
{
	var istrm = fs.OpenTextFile(args(0)); // offset.h
	var ostrm = fs.CreateTextFile(args(1)); // offset.inc

	ostrm.WriteLine("; offset.inc");
	ostrm.WriteLine("");

	var line;
	var newline;

	while(!istrm.AtEndOfStream){
		line = istrm.ReadLine();

		re=/\s+$/;
		newline = line.replace(re, "");
		re=/^#define\s+([0-9|a-z|A-Z|_]+)\s+(.+)$/;
		if(newline.search(re) == -1)
			continue;

		var value = newline.replace(re, "$2");
		re2=/^0x([0-9A-Fa-f]+)$/;
		if(value.search(re2) != -1)
			value = "0" + value.replace(re2, "$1") + "H";

		newline = newline.replace(re, "	$1	.equ	" + value);

		ostrm.WriteLine(newline);
	}
	istrm.Close();
	ostrm.Close();
}
