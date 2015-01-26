.PHONY: test

default: test

test:
	node_modules/mocha/bin/mocha --reporter spec --ui tdd
