native-hashset [![Build Status](https://travis-ci.org/iwhitfield/native-hashset.svg?branch=master)](https://travis-ci.org/iwhitfield/native-hashset)
==============

Provides a native hashset implementation for Node. Based around [node-hashset](https://github.com/inolen/node-hashset) but written using [nan](https://github.com/rvagg/nan) to provide better support for multiple Node systems (and the first io.js systems).

Typically when a set is needed in JavaScript, it may be sufficient to simply use a JavaScript object:

```
var set = {};

// add
set['foobar'] = true;

// contains
if (!set['foobar']) {
    // whatever
}

// iterator
Object.keys(set).forEach(function(k){
    console.log(k);
});
```

This works well for up to a few million items, but after that it starts to grind v8 down to a halt. It also has the negative of only allowing String types.

`native-hashset` implements a HashSet using [std::unsorted_set](http://en.cppreference.com/w/cpp/container/unordered_set) to enable working with extremely large sets. Naturally this set is strictly typed due to the nature of C++.

## Compatibility

This module is built on each commit with TravisCI on Node 0.8.x, 0.10.x and 0.11.x and the latest version of io.js. Due to a dependency on `node_internals.h` it will not compile correctly on Node 0.6.x (but if you're using that you should probably upgrade...). In order to maintain support throughout these versions the `nan` library is used for compatibility across varying Node versions.

## Setup

```
$ npm install native-hashset
```

## Quick Start

```
var HashSet = require('native-hashset');
var set = new HashSet.String();
set.add('foobar');
```

```
var HashSet = require('native-hashset');
var set = new HashSet.Integer();
set.add(1);
```

## HashSet

There are currently two HashSet constructors, `HashSet.Integer` and `HashSet.String`.

### add(value)

Adds `value` to the set. Throws an error if `value` is not the correct type for the set.

### buckets()

Returns the native bucket distribution of the unordered_set used under the hood.

### clear()

Removes all values from the set.

### contains(value)

Returns a boolean value indicating if the set contains `value`.

### count(value)

Returns an Integer count indicating if the set contains `value`. `1` if exists, `0` if not.

### empty()

Returns a boolean value indicating if the set contains any values.

### iterator()

Returns an iterator to iterate over the values in the set.

```
var it = set.iterator();

while (it.hasNext()) {
  console.log(it.next());
}
```

### keys()

Returns an Array containing all keys in the set. Use with caution and on smaller sets.

### remove(value)

Removes `value` from the set.

### size()

Returns the numbers of values in the set.

### type()

Returns the type of the current set.

## Tests

Basic tests are contained in the `test/` directory and are designed to be run using [Mocha](http://mochajs.org). They can be run using the Makefile via the commands below:

```
$ npm install
$ make test
```

## Credits

Thanks to [inolen](https://github.com/inolen) for his original HashSet [implementation](https://github.com/inolen/node-hashset) which inspired and provided the groundwork for this module.
