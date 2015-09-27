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
class HashSet : public Nan::ObjectWrap {
private:
    std::unordered_set<K> set;

    class Iterator : public Nan::ObjectWrap {
    private:
        const std::unordered_set<K> *set;
        typename std::unordered_set<K>::const_iterator iterator;

    public:
        Iterator(const std::unordered_set<K> *set) : set(set) {
            iterator = set->begin();
        }

        static Handle<Value> New(const std::unordered_set<K> *set) {
            Local<Object> obj = Nan::New<FunctionTemplate>(GetCtor)->GetFunction()->NewInstance();
            Iterator *it = new Iterator(set);

            it->Wrap(obj);

            return obj;
        }

        static NAN_METHOD(GetCtor){
            Nan::HandleScope scope;

            Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>();
            tpl->SetClassName(Nan::New<String>("HashSetIt").ToLocalChecked());
            tpl->InstanceTemplate()->SetInternalFieldCount(1);

            auto prototype = tpl->PrototypeTemplate();
            prototype->Set(Nan::New<String>("hasNext").ToLocalChecked(), Nan::New<FunctionTemplate>(HasNext));
            prototype->Set(Nan::New<String>("next").ToLocalChecked(), Nan::New<FunctionTemplate>(Next));

            info.GetReturnValue().Set(Nan::NewInstance(tpl->GetFunction()).ToLocalChecked());
        }

        static NAN_METHOD(HasNext) {
            Nan::HandleScope scope;

            Iterator *obj = Nan::ObjectWrap::Unwrap<Iterator>(info.Holder());

            info.GetReturnValue().Set(Nan::New<Boolean>(obj->iterator != obj->set->end()));
        }

        static NAN_METHOD(Next) {
            Nan::HandleScope scope;

            Iterator *obj = Nan::ObjectWrap::Unwrap<Iterator>(info.Holder());

            K value = *obj->iterator;
            obj->iterator++;

            info.GetReturnValue().Set(WrapValue(value));
        }
    };

    static NAN_METHOD(New) {
        Nan::HandleScope scope;

        HashSet *obj = new HashSet();

        obj->Wrap(info.Holder());

        info.GetReturnValue().Set(info.Holder());
    }

    static NAN_METHOD(Add) {
        Nan::HandleScope scope;

        HashSet *obj = Nan::ObjectWrap::Unwrap<HashSet>(info.Holder());

        if (info.Length() < 1) {
            return Nan::ThrowError("No key provided!");
        } else if(!IsA<K>(info[0])) {
            return Nan::ThrowTypeError("Invalid key type provided!");
        }

        K key = UnwrapValue<K>(info[0]);

        obj->set.insert(key);

        return;
    }

    static NAN_METHOD(Buckets) {
        Nan::HandleScope scope;

        HashSet *obj = Nan::ObjectWrap::Unwrap<HashSet>(info.Holder());
        Local<Array> buckets = Nan::New<Array>();

        for (unsigned i = 0; i < obj->set.bucket_count(); i++) {
            Local<Array> bucket = Nan::New<Array>();
            unsigned x = 0;
            for (auto it = obj->set.begin(i); it != obj->set.end(i); it++) {
                bucket->Set(x++, WrapValue(*it));
            }
            buckets->Set(i, bucket);
        }

        info.GetReturnValue().Set(buckets);
    }

    static NAN_METHOD(Clear) {
        Nan::HandleScope scope;

        HashSet *obj = Nan::ObjectWrap::Unwrap<HashSet>(info.Holder());

        obj->set.clear();

        return;
    }

    static NAN_METHOD(Contains) {
        Nan::HandleScope scope;

        HashSet *obj = Nan::ObjectWrap::Unwrap<HashSet>(info.Holder());

        if (info.Length() < 1) {
            return Nan::ThrowError("No key provided!");
        } else if(!IsA<K>(info[0])) {
            return Nan::ThrowTypeError("Invalid key type provided!");
        }

        K key = UnwrapValue<K>(info[0]);

        info.GetReturnValue().Set(Nan::New<Number>(obj->set.count(key) > 0));
    }

    static NAN_METHOD(Count) {
        Nan::HandleScope scope;

        HashSet *obj = Nan::ObjectWrap::Unwrap<HashSet>(info.Holder());

        if (info.Length() < 1) {
            return Nan::ThrowError("No key provided!");
        } else if(!IsA<K>(info[0])) {
            return Nan::ThrowTypeError("Invalid key type provided!");
        }

        K key = UnwrapValue<K>(info[0]);

        info.GetReturnValue().Set(Nan::New<Number>(obj->set.count(key)));
    }

    static NAN_METHOD(Empty){
        Nan::HandleScope scope;

        HashSet *obj = Nan::ObjectWrap::Unwrap<HashSet>(info.Holder());

        info.GetReturnValue().Set(obj->set.empty());
    }

    static NAN_METHOD(Iterator){
        Nan::HandleScope scope;

        HashSet *obj = Nan::ObjectWrap::Unwrap<HashSet>(info.Holder());

        info.GetReturnValue().Set(Iterator::New(&obj->set));
    }

    static NAN_METHOD(Keys){
        Nan::HandleScope scope;

        HashSet *obj = Nan::ObjectWrap::Unwrap<HashSet>(info.Holder());
        Local<Array> array = Nan::New<Array>();

        unsigned i = 0;
        for (const auto& elem: obj->set) {
            array->Set(i++, WrapValue(elem));
        }

        info.GetReturnValue().Set(array);
    }

    static NAN_METHOD(Remove) {
        Nan::HandleScope scope;

        HashSet *obj = Nan::ObjectWrap::Unwrap<HashSet>(info.Holder());

        if (info.Length() < 1) {
            return Nan::ThrowError("No key provided!");
        } else if (!IsA<K>(info[0])) {
            return Nan::ThrowTypeError("Invalid key type provided!");
        }

        K key = UnwrapValue<K>(info[0]);

        obj->set.erase(key);

        return;
    }

    static NAN_METHOD(Size){
        Nan::HandleScope scope;

        HashSet *obj = Nan::ObjectWrap::Unwrap<HashSet>(info.Holder());

        info.GetReturnValue().Set(Nan::New<Number>(obj->set.size()));
    }

    static NAN_METHOD(Type){
        Nan::HandleScope scope;
        info.GetReturnValue().Set(Nan::New<String>(GetType<K>()).ToLocalChecked());
    }

public:
    static NAN_METHOD(Init){
        Nan::HandleScope scope;

        Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(New);
        tpl->SetClassName(Nan::New<String>("HashSet").ToLocalChecked());
        tpl->InstanceTemplate()->SetInternalFieldCount(1);

        auto prototype = tpl->PrototypeTemplate();
        prototype->Set(Nan::New<String>("add").ToLocalChecked(), Nan::New<FunctionTemplate>(Add));
        prototype->Set(Nan::New<String>("buckets").ToLocalChecked(), Nan::New<FunctionTemplate>(Buckets));
        prototype->Set(Nan::New<String>("clear").ToLocalChecked(), Nan::New<FunctionTemplate>(Clear));
        prototype->Set(Nan::New<String>("contains").ToLocalChecked(), Nan::New<FunctionTemplate>(Contains));
        prototype->Set(Nan::New<String>("count").ToLocalChecked(), Nan::New<FunctionTemplate>(Count));
        prototype->Set(Nan::New<String>("empty").ToLocalChecked(), Nan::New<FunctionTemplate>(Empty));
        prototype->Set(Nan::New<String>("iterator").ToLocalChecked(), Nan::New<FunctionTemplate>(Iterator));
        prototype->Set(Nan::New<String>("keys").ToLocalChecked(), Nan::New<FunctionTemplate>(Keys));
        prototype->Set(Nan::New<String>("remove").ToLocalChecked(), Nan::New<FunctionTemplate>(Remove));
        prototype->Set(Nan::New<String>("size").ToLocalChecked(), Nan::New<FunctionTemplate>(Size));
        prototype->Set(Nan::New<String>("type").ToLocalChecked(), Nan::New<FunctionTemplate>(Type));

        info.GetReturnValue().Set(Nan::NewInstance(tpl->GetFunction()).ToLocalChecked());
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
    return Nan::New<Number>(value);
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
    return Nan::New<String>(value.c_str()).ToLocalChecked();
}

#endif
