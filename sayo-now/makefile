CC = g++
PROG = prog
INC = -I sayo-lib
ifeq ($(OS),Windows_NT)
    EXE = sayo-now.exe
else
    EXE = sayo-now
endif

.RECIPEPREFIX = >
.PHONY: clean

./out/$(PROG).json: ./out/$(PROG).out ./translate_config/*
> python3 process.py ./out/$(PROG).out ./out/$(PROG).json

./out/$(PROG).out: ./out/$(PROG).in
> ./$(EXE) $^ -o $@

./out/$(PROG).in: $(PROG).cpp ./sayo-lib/*
> $(CC) $< -o $@ -E -DSAYO_NOW $(INC)

clean:
> rm out/*