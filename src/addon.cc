#include "hashset.h"

using namespace v8;

void Init(Handle<Object> exports) {
	exports->Set(NanNew<String>("Integer"), NanNew<FunctionTemplate>(HashSet<int64_t>::Init)->GetFunction());
	exports->Set(NanNew<String>("String"), NanNew<FunctionTemplate>(HashSet<std::string>::Init)->GetFunction());
}

NODE_MODULE(addon, Init);
