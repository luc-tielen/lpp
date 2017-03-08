
all: debug release

debug:
	./scripts/build_debug.sh

release:
	./scripts/build_release.sh

clean:
	./scripts/clean.sh

tests:
	./scripts/run_tests.sh

.PHONY: all debug release clean tests

