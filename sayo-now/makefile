CC = g++
PROG = prog
INC = -I sayo-lib

.RECIPEPREFIX = >
.PHONY: clean

./out/$(PROG).json: ./out/$(PROG).out ./translate_config/*
> python3 process.py ./out/$(PROG).out ./out/$(PROG).json

./out/$(PROG).out: ./out/$(PROG).in
> ./sayo-now.exe $^ -o $@

./out/$(PROG).in: $(PROG).cpp
> $(CC) $^ -o $@ -E -DSAYO_NOW $(INC)

clean:
> rm -rf out/*