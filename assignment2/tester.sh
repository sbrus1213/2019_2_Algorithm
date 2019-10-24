INPUTS=$(ls *_input.txt | grep -v "result")
rm -f result*
if [ $# -ne 0 ]
then
	echo "Start testing..."
	for input in $INPUTS
	do
		./$1 $input 1 
		./$1 $input 2 
		./$1 $input 3 
		./$1 $input 4 
		echo "make result file for "$input
	done
	echo "Finish testing!!!"
else 
	echo "please write [binary file name]"
fi
