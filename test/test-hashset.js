var assert = require('assert');
var HashSet = require('../index');

[
    { type: 'String', keys: ['key1', 'key2', 'invalid'] },
    { type: 'Integer', keys: [-13, 79, 42] }
].forEach(function (it) {
    var type = it.type;
    var keys = it.keys;
    var key1 = it.keys[0];
    var key2 = it.keys[1];
    var missing = it.keys[2];

    suite('Hashset-' + type, function() {
        var set;

        setup(function() {
            set = new HashSet[type]();
        });

        test('add()', function () {
            set.add(key1);
            assert(set.contains(key1));
            assert(!set.contains(missing));
        });

        test('buckets()', function () {
            it.keys.forEach(function(k){
                set.add(k);
            });

            var buckets = set.buckets();

            assert(buckets.length > 0);

            var length = 0;

            buckets.forEach(function(b){
                length += b.length;
            });

            assert.equal(length, it.keys.length);
        });

        test('clear()', function () {
            assert.equal(set.size(), 0);
            set.add(key1);
            assert.equal(set.size(), 1);
            set.clear();
            assert.equal(set.size(), 0);
        });

        test('contains()', function () {
            set.add(key1);
            assert(set.contains(key1));
            assert(!set.contains(missing));
            set.add(key2);
            assert(set.contains(key2));
            assert(!set.contains(missing));
        });

        test('count()', function () {
            set.add(key1);
            assert.equal(set.count(key1), 1);
            assert.equal(set.count(missing), 0);
            set.add(key2);
            assert.equal(set.count(key2), 1);
            assert.equal(set.count(missing), 0);
        });

        test('empty()', function () {
            assert(set.empty());
            set.add(key1);
            assert(!set.empty());
        });

        test('keys()', function () {
            set.add(key1);
            set.add(key2);

            var arr = set.keys();

            assert.equal(arr[0], key2);
            assert.equal(arr[1], key1);
        });

        test('iterator()', function () {
            set.add(key1);
            set.add(key2);

            var it = set.iterator();

            assert(it.hasNext());
            assert(keys.indexOf(it.next()) !== -1);

            assert(it.hasNext());
            assert(keys.indexOf(it.next()) !== -1);

            assert(!it.hasNext());
        });

        test('remove()', function () {
            set.add(key1);
            assert(set.contains(key1));
            set.remove(key1);
            assert(!set.contains(key1));
        });

        test('size()', function () {
            assert.equal(set.size(), 0);
            set.add(key1);
            assert.equal(set.size(), 1);
            set.add(key2);
            assert.equal(set.size(), 2);
        });

        test('type()', function () {
            assert.equal(set.type(), type);
        });
    });
});