#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

#include <memory>
namespace wolver {

class WolNode;
class WolComplexNode;
class WolConstValue;
class WolValue;
class WolValueImpl;
class WolConstValueImpl;
class WolRangeValueImpl;
class WolBoolValueImpl;
class WolUnionValueImpl;
class WolConcatValueImpl;

#define WOL_FIND_AND_NODE_CONTRADICTION_LIMT 20

typedef std::shared_ptr<WolNode> WolNodeSptr;
typedef std::shared_ptr<WolComplexNode> WolComplexNodeSptr;
typedef std::weak_ptr<WolNode> WolNodeWptr;
typedef std::shared_ptr<WolConstValue> WolConstValueSptr;
typedef std::shared_ptr<WolValue> WolValueSptr;
typedef std::weak_ptr<WolValue> WolValueWptr;
typedef std::shared_ptr<WolValueImpl> WolValueImplSptr;
typedef std::weak_ptr<WolValueImpl> WolValueImplWptr;
typedef std::shared_ptr<WolBoolValueImpl> WolBoolValueImplSptr;
typedef std::weak_ptr<WolBoolValueImpl> WolBoolValueImplWptr;
typedef std::shared_ptr<WolConstValueImpl> WolConstValueImplSptr;
typedef std::weak_ptr<WolConstValueImpl> WolConstValueImplWptr;
typedef std::shared_ptr<WolRangeValueImpl> WolRangeValueImplSptr;
typedef std::weak_ptr<WolRangeValueImpl> WolRangeValueImplWptr;
typedef std::shared_ptr<WolUnionValueImpl> WolUnionValueImplSptr;
typedef std::weak_ptr<WolUnionValueImpl> WolUnionValueImplWptr;
typedef std::shared_ptr<WolConcatValueImpl> WolConcatValueImplSptr;
typedef std::weak_ptr<WolConcatValueImpl> WolConcatValueImplWptr;

}

#endif
