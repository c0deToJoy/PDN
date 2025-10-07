OSCACA_FLAGS?=

todo:
	@grep "STUDENT_TODO" *.c
	@echo ""
	@echo "Work through all of the STUDENT_TODO"
	@echo "run: 'make test' for the demo of the instructions."
	@echo "run: 'make measure-osaca' to see the hardware simulator."


test: build
	./run.x

build: examples_simd_avx.c
	gcc -std=c99 -g -O1 -mavx2 -mfma examples_simd_avx.c  -o ./run.x



# use osaca to simulate the code
measure-osaca: venv examples_simd_avx.c
	gcc -std=c99 -O1 -mavx2 -mfma examples_simd_avx.c -DUSE_OSACA=1 -S 
	. venv/bin/activate;  osaca $(OSACA_FLAGS)  --ignore-unknown examples_simd_avx.s  > examples_simd_avx.osaca
	cat examples_simd_avx.osaca

# Create a virtualenv for all of the python tooling
venv: venv/touchfile
venv/touchfile: requirements.txt
	test -d venv || python3 -m venv venv
	. venv/bin/activate; pip install -Ur requirements.txt
	touch venv/touchfile


clean:
	rm -f *.x *~ *.o *.x *.s *.osaca

clean-all:
	rm -rf venv
