#include "hashset.h"

using namespace v8;

void Init(Handle<Object> exports) {
	exports->Set(Nan::New<String>("Integer").ToLocalChecked(), Nan::New<FunctionTemplate>(HashSet<int64_t>::Init)->GetFunction());
	exports->Set(Nan::New<String>("String").ToLocalChecked(), Nan::New<FunctionTemplate>(HashSet<std::string>::Init)->GetFunction());
}

NODE_MODULE(addon, Init);
