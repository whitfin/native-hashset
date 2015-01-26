#ifndef __HASHSET_H__
#define __HASHSET_H__

#include <nan.h>
#include <node.h>
#include <v8.h>
#include <unordered_set>

using namespace v8;

template <typename T> bool IsA(const Local<Value> &arg);
template <typename T> std::string GetType();
template <typename T> T UnwrapValue(const Local<Value> &arg);
template <typename T> Handle<Value> WrapValue(const T &value);

template <typename K>
class HashSet : public node::ObjectWrap {
private:
	std::unordered_set<K> set;

	class Iterator : public node::ObjectWrap {
	private:
		const std::unordered_set<K> *set;
		typename std::unordered_set<K>::const_iterator iterator;

	public:
		Iterator(const std::unordered_set<K> *set) : set(set) {
			iterator = set->begin();
		}

		static Handle<Value> New(const std::unordered_set<K> *set) {
			Local<Object> obj = NanNew<FunctionTemplate>(GetCtor)->GetFunction()->NewInstance();
			Iterator *it = new Iterator(set);

			it->Wrap(obj);

			return obj;
		}

		static NAN_METHOD(GetCtor){
			NanScope();

			Local<FunctionTemplate> tpl = NanNew<FunctionTemplate>();
			tpl->SetClassName(NanNew<String>("HashSetIt"));
			tpl->InstanceTemplate()->SetInternalFieldCount(1);

			auto prototype = tpl->PrototypeTemplate();
			prototype->Set(NanNew<String>("hasNext"), NanNew<FunctionTemplate>(HasNext));
			prototype->Set(NanNew<String>("next"), NanNew<FunctionTemplate>(Next));

			NanReturnValue(NanNew(tpl->GetFunction())->NewInstance());
		}

		static NAN_METHOD(HasNext) {
			NanScope();

			Iterator *obj = node::ObjectWrap::Unwrap<Iterator>(args.Holder());

			NanReturnValue(NanNew<Boolean>(obj->iterator != obj->set->end()));
		}

		static NAN_METHOD(Next) {
			NanScope();

			Iterator *obj = node::ObjectWrap::Unwrap<Iterator>(args.Holder());

			K value = *obj->iterator;
			obj->iterator++;

			NanReturnValue(WrapValue(value));
		}
	};

	static NAN_METHOD(New) {
		NanScope();

		HashSet *obj = new HashSet();

		obj->Wrap(args.Holder());

		NanReturnValue(args.Holder());
	}

	static NAN_METHOD(Add) {
		NanScope();

		HashSet *obj = node::ObjectWrap::Unwrap<HashSet>(args.Holder());

		if (args.Length() < 1) {
			return NanThrowError("No key provided!");
		} else if(!IsA<K>(args[0])) {
			return NanThrowTypeError("Invalid key type provided!");
		}

		K key = UnwrapValue<K>(args[0]);

		obj->set.insert(key);

		NanReturnUndefined();
	}

	static NAN_METHOD(Buckets) {
		NanScope();

		HashSet *obj = node::ObjectWrap::Unwrap<HashSet>(args.Holder());
		Local<Array> buckets = NanNew<Array>();

		for (unsigned i = 0; i < obj->set.bucket_count(); i++) {
			Local<Array> bucket = NanNew<Array>();
			unsigned x = 0;
			for (auto it = obj->set.begin(i); it != obj->set.end(i); it++) {
				bucket->Set(x++, WrapValue(*it));
			}
			buckets->Set(i, bucket);
		}

		NanReturnValue(NanNew(buckets));
	}

	static NAN_METHOD(Clear) {
		NanScope();

		HashSet *obj = node::ObjectWrap::Unwrap<HashSet>(args.Holder());

		obj->set.clear();

		NanReturnUndefined();
	}

	static NAN_METHOD(Contains) {
		NanScope();

		HashSet *obj = node::ObjectWrap::Unwrap<HashSet>(args.Holder());

		if (args.Length() < 1) {
			return NanThrowError("No key provided!");
		} else if(!IsA<K>(args[0])) {
			return NanThrowTypeError("Invalid key type provided!");
		}

		K key = UnwrapValue<K>(args[0]);

		NanReturnValue(NanNew<Number>(obj->set.count(key) > 0));
	}

	static NAN_METHOD(Count) {
		NanScope();

		HashSet *obj = node::ObjectWrap::Unwrap<HashSet>(args.Holder());

		if (args.Length() < 1) {
			return NanThrowError("No key provided!");
		} else if(!IsA<K>(args[0])) {
			return NanThrowTypeError("Invalid key type provided!");
		}

		K key = UnwrapValue<K>(args[0]);

		NanReturnValue(NanNew<Number>(obj->set.count(key)));
	}

	static NAN_METHOD(Empty){
		NanScope();

		HashSet *obj = node::ObjectWrap::Unwrap<HashSet>(args.Holder());

		NanReturnValue(NanNew<Boolean>(obj->set.empty()));
	}

	static NAN_METHOD(Iterator){
		NanScope();

		HashSet *obj = node::ObjectWrap::Unwrap<HashSet>(args.Holder());

		NanReturnValue(NanNew(Iterator::New(&obj->set)));
	}

	static NAN_METHOD(Keys){
		NanScope();

		HashSet *obj = node::ObjectWrap::Unwrap<HashSet>(args.Holder());
		Local<Array> array = NanNew<Array>();

		unsigned i = 0;
		for (const auto& elem: obj->set) {
			array->Set(i++, WrapValue(elem));
		}

		NanReturnValue(NanNew(array));
	}

	static NAN_METHOD(Remove) {
		NanScope();

		HashSet *obj = node::ObjectWrap::Unwrap<HashSet>(args.Holder());

		if (args.Length() < 1) {
			return NanThrowError("No key provided!");
		} else if (!IsA<K>(args[0])) {
			return NanThrowTypeError("Invalid key type provided!");
		}

		K key = UnwrapValue<K>(args[0]);

		obj->set.erase(key);

		NanReturnUndefined();
	}

	static NAN_METHOD(Size){
		NanScope();

		HashSet *obj = node::ObjectWrap::Unwrap<HashSet>(args.Holder());

		NanReturnValue(NanNew<Number>(obj->set.size()));
	}

    static NAN_METHOD(Type){
        NanScope();
        NanReturnValue(NanNew<String>(GetType<K>()));
    }

public:
	static NAN_METHOD(Init){
		NanScope();

		Local<FunctionTemplate> tpl = NanNew<FunctionTemplate>(New);
		tpl->SetClassName(NanNew<String>("HashSet"));
		tpl->InstanceTemplate()->SetInternalFieldCount(1);

		auto prototype = tpl->PrototypeTemplate();
		prototype->Set(NanNew<String>("add"), NanNew<FunctionTemplate>(Add));
		prototype->Set(NanNew<String>("buckets"), NanNew<FunctionTemplate>(Buckets));
		prototype->Set(NanNew<String>("clear"), NanNew<FunctionTemplate>(Clear));
		prototype->Set(NanNew<String>("contains"), NanNew<FunctionTemplate>(Contains));
		prototype->Set(NanNew<String>("count"), NanNew<FunctionTemplate>(Count));
		prototype->Set(NanNew<String>("empty"), NanNew<FunctionTemplate>(Empty));
		prototype->Set(NanNew<String>("iterator"), NanNew<FunctionTemplate>(Iterator));
		prototype->Set(NanNew<String>("keys"), NanNew<FunctionTemplate>(Keys));
		prototype->Set(NanNew<String>("remove"), NanNew<FunctionTemplate>(Remove));
		prototype->Set(NanNew<String>("size"), NanNew<FunctionTemplate>(Size));
        prototype->Set(NanNew<String>("type"), NanNew<FunctionTemplate>(Type));

		NanReturnValue(NanNew(tpl->GetFunction())->NewInstance());
	}
};

// int64_t specialization
template <>
bool IsA<int64_t>(const Local<Value> &arg) {
	return arg->IsNumber();
}

template <>
std::string GetType<int64_t>() {
   return "Integer";
}

template <>
int64_t UnwrapValue<int64_t>(const Local<Value> &arg) {
	return arg->IntegerValue();
}

template <>
Handle<Value> WrapValue<int64_t>(const int64_t &value) {
	return NanNew<Number>(value);
}

// std::string specialization
template <>
bool IsA<std::string>(const Local<Value> &arg) {
	return arg->IsString();
}

template <>
std::string GetType<std::string>() {
    return "String";
}

template <>
std::string UnwrapValue<std::string>(const Local<Value> &arg) {
	Local<String> local = Local<String>::Cast(arg);
	String::Utf8Value utf(local);
	return *utf;
}

template <>
Handle<Value> WrapValue<std::string>(const std::string &value) {
	return NanNew<String>(value.c_str());
}

#endif
