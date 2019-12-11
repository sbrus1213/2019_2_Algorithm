INPUTS=$(ls *_input.txt)

echo "Start check ..."
for input in $INPUTS
do
	echo "Compare "$input" and "$input".zz.yy"
	./check $input
done
echo "Finish Check !"
