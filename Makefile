ORIG ?= orig
all: verify
verify:
	sha1sum -c $(ORIG)/checksum.sha1
