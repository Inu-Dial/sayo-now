CC = g++
PROG = prog

.RECIPEPREFIX = >
.PHONY: clean

./out/$(PROG).json: ./out/$(PROG).out ./translate_config/*
> ./translate $<

./out/$(PROG).out: ./out/$(PROG).in
> ./sayo-now $^ -o $@

./out/$(PROG).in: $(PROG).cpp
> mkdir -p out
> $(CC) $^ -o $@ -E -DSAYO_NOW

clean:
> rm -rf out