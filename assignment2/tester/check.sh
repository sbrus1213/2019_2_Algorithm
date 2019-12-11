INPUTS=$(ls result_*)

for input in $INPUTS
do
	echo $input
	echo $(sed -n 4p $input)
done
