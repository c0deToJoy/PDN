todo:
	@grep -n -A 1 -B 2 STUDENT_TODO hw_code.c || true
	@grep -n FIXME hw_code.c || true
	@echo -e "\n\n"
	@echo -e "INSTRUCTIONS: Fix the code until every test passes."
	@echo -e "              You will also diagram the data movement in a separate pdf."
	@echo -e "run: make test"

test: build
	./run_hw.x | tee results.txt
	@grep --color PASS results.txt || true
	@grep --color FAIL results.txt || true
build:
	gcc -std=c99 -g -O1 -mavx2 -mfma -fopenmp hw_code.c  -o ./run_hw.x

package_for_submission: test
	tar czvf submit-to-canvas.tar.gz hw_code.c results.txt

clean:
	rm -f *.x *~ *.o *.x results.txt  submit-to-canvas.tar.gz
