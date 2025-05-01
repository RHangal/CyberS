final: input.txt 15char.txt lipsum.txt
	$(MAKE) clean
input.txt 15char.txt lipsum.txt: gen_keys
	bigrsa/bigrsa.o -e bigrsa/test/$@ $@.encrypted
	bigrsa/bigrsa.o -d $@.encrypted $@.decrypted
	diff bigrsa/test/$@ $@.decrypted
gen_keys:
	bigrsa/bigkey.o
