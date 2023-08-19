files := trivium_noinit_notsat trivium_noinit_1 trivium_noinit_2 trivium_noinit_3 trivium_noinit_4  

.PHONY: all clean_sat print

all: print $(foreach fl, $(files), $(fl).cnf) $(foreach fl, $(files), $(fl).sat)

print: $(foreach fl, $(files), $(fl).sat)
	$(foreach var, $(files), grep "^s" $(var).sat;) 

%.cnf: %.c
	cbmc $< --dimacs --outfile $@

%.sat: %.cnf
	~/SATsolvers/kissat-3.1.0-linux-amd64 $< > $@ || /bin/true

clean:
	rm $(foreach fl, $(files), $(fl).sat) $(foreach fl, $(files), $(fl).cnf)
