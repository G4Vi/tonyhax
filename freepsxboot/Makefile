
# Thanks to whoever made https://devhints.io/makefile!

include ../variables.mk

.PHONY: clean FreePSXBoot/builder/builder

all: $(FREEPSXBOOT_IMAGES)

clean:
	$(RM) $(FREEPSXBOOT_IMAGES) FreePSXBoot/builder/builder

tonyhax_%.mcd: ../loader/tonyhax.exe FreePSXBoot/builder/builder
	FreePSXBoot/builder/builder -fastload -bios $(patsubst tonyhax_bios_v%.mcd,%,$@) -in ../loader/tonyhax.exe -out $@

FreePSXBoot/builder/builder:
	make -C FreePSXBoot/builder all
