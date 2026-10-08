GAME_DIR ?= game
all: verify
verify:
	sha1sum -c checksum.sha1
