#include "wolvalueimpl.h"
#include "wolevalfactoryimpl.h"
#include "../wolvalue.h"
#include "../wolutil.h"
#include "../wolmgr.h"
#include "../wollog.h"
#include "../wolevalfactory.h"
#include <boost/dynamic_bitset.hpp>
#include <boost/ref.hpp>
#include "../common.h"
#include "memory.h"
#include <random>


namespace wolver
{

  WolValue::WolConstType
  WolConstValueImpl::getConstType () {

    bool is_zero_present = false;
    bool is_one_present = false;
    int num_ones = 0;

    for (boost::dynamic_bitset<>::size_type i = 0; i < _value.size (); ++i) {
      if ((bool) _value[i] == true) {
	is_one_present = true;
	num_ones++;
      }
      if ((bool) _value[i] == false) {
	is_zero_present = true;
      }
    }

    if (_value.size () == 1 && is_one_present)
      return WolValue::WOL_SPECIAL_CONST_ONE_ONES;
    else if (!is_zero_present)
      return WolValue::WOL_SPECIAL_CONST_ONES;
    else if (!is_one_present)
      return WolValue::WOL_SPECIAL_CONST_ZERO;
    else if (num_ones == 1 && (bool) _value[0] == true)
      return WOL_SPECIAL_CONST_ONE;
    else
      return WOL_SPECIAL_CONST_NONE;
  }

  WolValueImplSptr
  WolBoolValueImpl::getNotValueInt () {
    DEBUG4_MSG << "BOOL " << this->getStringRep()
               << " NOT";
    WolBoolValueImplSptr retValue = makeBoolImpl(!_value);
    DEBUG4_MSG << "RESULT = " << retValue->getStringRep();
    return retValue;
  }

  WolValueImplSptr
  WolConstValueImpl::getNotValueInt () {
	DEBUG4_MSG << "CONST " << this->getStringRep()
	           << " NOT";
    WolConstValueImplSptr retValue = makeConstImpl(~_value);
    DEBUG4_MSG << "RESULT = " << retValue->getStringRep();
    return retValue;
  }

  std::string
  WolConstValueImpl::getStringRep () {

    std::string retValue;
    boost::to_string (_value, retValue);
    return retValue;
  }

  // split in the string rep of the value(MSB comes first); positioning start from 1
  WolValueImplSptr
    WolConstValueImpl::split (int position) {

    int prec = _value.size();
    assert(position > 0);
    assert(position <= prec - 1);

    DEBUG4_MSG << "CONST " << this->getStringRep()
    	       << " SPLIT " << position ;

    //rightValue = _value(0 to _prec - position - 1)
    dbitset rightValue = createSubsetOfDynamicBitSet (_value, 0, prec - position - 1);

    //leftValue = _value(prec - position to _prec - 1)
    dbitset leftValue = createSubsetOfDynamicBitSet (_value, prec - position, prec - 1);

    auto leftConst = std::make_shared<WolConstValueImpl> (leftValue);
    auto rightConst = std::make_shared<WolConstValueImpl>(rightValue);
    auto concatConst = std::make_shared<WolConcatValueImpl> ();
    concatConst->addValue (leftConst);
    concatConst->addValue (rightConst);

    DEBUG4_MSG << "RESULT = " << concatConst->getStringRep();
    return concatConst;
  }


  std::string
  WolRangeValueImpl::getStringRep () {

    std::string lowValue, highValue, retValue;
    boost::to_string (_lowValue, lowValue);
    boost::to_string (_highValue, highValue);
    retValue = "[" + lowValue + ", " + highValue + "]";
    return retValue;
  }



  /*
   * _prec ---> length of the vector indices form 0 to _prec - 1
   *  position is defined  from the start of highest significant bit
   *  highest significant bit will have position 1.
   *
   * rightLowValue = _lowValue(0 to _prec - position - 1)
   * leftLowValue = _lowValue(_prec - position to _prec - 1)
   * rightHighValue = _highValue(0 to _prec - position - 1)
   * leftHighValue = _highValue(_prec - position to _prec - 1)
   *
   * split would contain following parts:
   * 1)  leftLowValue . [rightLowValue, -1]
   * 2)  [leftLowValue + 1, leftHighValue - 1]. [0, -1]
   * 3)  leftHighValue. [0, rightHighValue]
   */
  //TODO: consider splitting full range.
  WolValueImplSptr
  WolRangeValueImpl::split (int position) {

	  assert(_lowValue < _highValue);
	  assert(position > 0);
	  assert(position <= _prec - 1);

	  DEBUG4_MSG << "RANGE " << this->getStringRep()
	      	     << " SPLIT " << position ;

	  //rightLowValue = _lowValue(0 to _prec - position - 1)
	  dbitset rightLowValue = createSubsetOfDynamicBitSet (_lowValue, 0, _prec - position - 1);

	  //leftLowValue = _lowValue(_prec - position to _prec - 1)
	  dbitset leftLowValue = createSubsetOfDynamicBitSet (_lowValue, _prec - position, _prec - 1);


	  //rightHighValue = _highValue(0 to _prec - position - 1)
	  dbitset rightHighValue = createSubsetOfDynamicBitSet (_highValue, 0, _prec - position - 1);

	  //leftHighValue = _highValue(_prec - position to _prec - 1)
	  dbitset leftHighValue = createSubsetOfDynamicBitSet (_highValue, _prec - position, _prec - 1);

	  if (leftLowValue == leftHighValue) {
		  // create leftLowValue . [righLowValue, rightHighValue]
		  auto rightRange = std::make_shared<WolRangeValueImpl> (rightLowValue,
				  rightHighValue);
		  auto leftConst = std::make_shared<WolConstValueImpl> (leftLowValue);
		  auto concatRange = std::make_shared<WolConcatValueImpl> ();
		  concatRange->addValue (leftConst);
		  concatRange->addValue (rightRange);

		  DEBUG4_MSG << "RESULT = " << concatRange->getStringRep();
		  return concatRange;
	  }
	  else {
		  int rprec = rightLowValue.size ();
		  auto result = std::make_shared<WolUnionValueImpl> ();

		  // create first =  leftLowValue . [rightLowValue, -1]
		  auto leftConst1 = std::make_shared<WolConstValueImpl> (leftLowValue);

		  if (rightLowValue == dbitset(rprec, (unsigned) -1)) {
              auto constant = std::make_shared<WolConstValueImpl> (_lowValue);
              result->addValue(constant);
  		  }
		  else {
			  auto rightRange1 = std::make_shared<WolRangeValueImpl> (
					  rightLowValue, dbitset (rprec, (unsigned) -1));

			  auto concat1 = std::make_shared<WolConcatValueImpl> ();
			  concat1->addValue (leftConst1);
			  concat1->addValue (rightRange1);
			  result->addValue (concat1);
		  }

		  if (bitSetInc (leftLowValue) > leftHighValue) {

			  auto concat2 = std::make_shared<WolConcatValueImpl> ();

			  if (bitSetInc (leftLowValue) == bitSetDec (leftHighValue)) {
				  // create second = leftLowValue + 1 . [0, -1]
				  auto leftConst2 = std::make_shared<WolConstValueImpl> (
						  bitSetInc (leftLowValue));
				  concat2->addValue (leftConst2);
			  }
			  else {
				  // create second = [leftLowValue + 1, leftHighValue - 1]. [0, -1]
				  auto leftRange2 = std::make_shared<WolRangeValueImpl> (
						  bitSetInc (leftLowValue), bitSetDec (leftHighValue));
				  concat2->addValue (leftRange2);
			  }
			  auto rightRange2 = std::make_shared<WolRangeValueImpl> (rprec);
			  concat2->addValue (rightRange2);
			  result->addValue (concat2);
		  }

		  // create third = leftHighValue. [0, rightHighValue]
		  auto leftConst3 = std::make_shared<WolConstValueImpl> (leftHighValue);

		  WolValueImplSptr rightRange3 = nullptr;
		  if( rightHighValue != dbitset(rprec, 0))
		      rightRange3 = std::make_shared<WolRangeValueImpl> (
				  dbitset (rprec, 0), rightHighValue);
		   else
			   rightRange3 = std::make_shared<WolConstValueImpl> (rightHighValue);
		  auto concat3 = std::make_shared<WolConcatValueImpl> ();
		  concat3->addValue (leftConst3);
		  concat3->addValue (rightRange3);
		  result->addValue (concat3);

		  DEBUG4_MSG << "RESULT = " << result->getStringRep();
		  return result;
	  }

	  assert(0);
	  return nullptr;
  }

  bool
  WolRangeValueImpl::isFull() {

    bool result = _lowValue.none() && (~_highValue).none();

    return result;
  }

  int
  WolRangeValueImpl::OptimalSplitPosition() {

	  assert(!this->isFull());

	  //_lowValue, _highValue
	  int position = 0;
	  int size = _lowValue.size();
	  for (int i = 0; i < size; i++) {
		  if (_lowValue[i] == _highValue[i])
			  position++;
		  else
			  break;
	  }

	  return position;
  }

  bool
  WolRangeValueImpl::checkConsistency(){

    return _lowValue < _highValue;

  }

  WolValueImplSptr
  WolRangeValueImpl::getNotValueInt() {

	DEBUG4_MSG << "RANGE " << this->getStringRep()
	  	       << " NOT ";

    if (isFull()) {
    	DEBUG4_MSG << "RESULT = " << this->getStringRep();
    	return shared_from_this();
    }
    assert(checkConsistency());

    int position = OptimalSplitPosition();
    if (position == 0) position = _prec/2;
    auto rangeSplit = split(position);

    WolValueImplSptr retValue = rangeSplit->getNotValueInt();
    DEBUG4_MSG << "RESULT = " << retValue->getStringRep();
    return retValue;
  }

  WolValueSptr
  WolRangeValueImpl::getRandomValue() {
    dbitset rand = bitSetRand(_lowValue, _highValue);
    auto retValue = std::make_shared<WolConstValueImpl> (rand);
    return retValue;
  }

  std::string
  WolUnionValueImpl::getStringRep () {

    std::stringstream ss;
    std::string value;

    ss << "<U>";
    for (auto i : _values)
      ss << i->getStringRep() ;
    ss << "<U>";

    return ss.str ();
  }

  WolValueImplSptr
  WolUnionValueImpl::split(int position) {

	  DEBUG4_MSG << "UNION " << this->getStringRep()
	  	  	     << " SPLIT " << position;

	  WolUnionValueImplSptr result = makeUnionImpl();
	  for (auto i : _values){
		  WolValueImplSptr split = i->split(position);

		  WolValueImplType type = split->getImplType();
		  assert(type == WOL_VALUE_IMPL_CONCAT_TYPE ||
				  type == WOL_VALUE_IMPL_UNION_TYPE );

		  if (type == WOL_VALUE_IMPL_CONCAT_TYPE) {
			  result->addValue(split);
		  }
		  else {
			  WolUnionValueImplSptr union_val = dUnionCast(split);
			  for (auto it = union_val->begin(); it != union_val->end(); it++) {
				  result->addValue(*it);
			  }
		  }
	  }

	  DEBUG4_MSG << "RESULT = " << result->getStringRep();
	  return result;
  }

  void
  WolUnionValueImpl::addValue (WolValueImplSptr value) {
    if (!_prec) _prec = value->getPrecision();
    assert(_prec == value->getPrecision());
    _values.push_back (value);
  }

  WolValueImplSptr
  WolUnionValueImpl::getSlices(int upper, int lower) {
    //TODO: need for optimizations

    WolUnionValueImplSptr retValue = makeUnionImpl();
    for(auto i : _values) {

        WolValueImplSptr temp = i->getSlices(upper, lower);
        if (temp->isRange() && temp->isFull())
          return temp;
        retValue->addValue(temp);
    }

    return retValue;
  }

  dbitset
  WolUnionValueImpl::getLowValue() {

   dbitset result = _values[0]->getLowValue();
   for (auto i : _values) {
     dbitset temp = i->getLowValue();
     if (result < temp) result = temp;
   }

    return result;
  }

  dbitset
  WolUnionValueImpl::getHighValue() {

    dbitset result = _values[0]->getHighValue();
    for (auto i : _values) {
      dbitset temp = i->getHighValue();
      if (result > temp) result = temp;
    }

    return result;
  }

  WolValueImplSptr
  WolUnionValueImpl::getNotValueInt() {

	DEBUG4_MSG << "UNION " << this->getStringRep()
	  	  	   << " NOT ";
    WolUnionValueImplSptr retValue = makeUnionImpl();
    for (auto i :_values) {
      retValue->addValue(i->getNotValueInt());
    }

    DEBUG4_MSG << "RESULT = " << retValue->getStringRep();
    return retValue;
  }

  bool
  WolUnionValueImpl::containsValue(dbitset const_val) {

    for(auto i : _values) {
      if (i->containsValue(const_val))
	return true;
    }

    return false;
  }

  WolValueSptr
  WolUnionValueImpl::getRandomValue() {
    int size = _values.size();

    //TODO: Need to consider weighted distribution
    std::default_random_engine generator;
    std::uniform_int_distribution<int> dist(0, size -1);

    int randIndex = dist(generator);

    return _values[randIndex]->getRandomValue();
  }

  std::string
  WolConcatValueImpl::getStringRep () {

    std::stringstream ss;
    std::string value;

    ss << "<C>";
    for (auto i : _values)
      ss << i->getStringRep ();
    ss << "<C>";

    return ss.str ();
  }

  void
  WolConcatValueImpl::addValue (WolValueImplSptr value) {
    _prec += value->getPrecision();
    _values.push_back (value);
  }

  WolValueImplSptr
  WolConcatValueImpl::split(int position) {

	  DEBUG4_MSG << "CONCAT " << this->getStringRep()
	      	  	 << " SPLIT " << position;

	  assert(position > 0);
	  assert(position < _prec);

	  //check if split is required
	  int sumPrecision = 0;
	  bool split = true;
	  WolValueImplSptr toSplit;
	  int index = 0;
	  for (auto i : _values) {
		  sumPrecision += i->getPrecision();
		  if (sumPrecision == position) {
			  split = false;
			  break;
		  }
		  else if (sumPrecision > position) {
			  toSplit = i;
			  break;
		  }
		  index++;
	  }

	  //if the data already has a split at position
	  if (!split) {
		  DEBUG4_MSG << "RESULT =" << this->getStringRep();
		  return shared_from_this();
	  }

	  int newPosition = position - (sumPrecision - toSplit->getPrecision());
	  WolValueImplSptr subSplit = toSplit->split(newPosition);

	  WolValueImplType type = subSplit->getImplType();
	  assert(type == WOL_VALUE_IMPL_CONCAT_TYPE ||
			  type == WOL_VALUE_IMPL_UNION_TYPE );


	  if (type == WOL_VALUE_IMPL_CONCAT_TYPE) {

		  WolConcatValueImplSptr result = makeConcatImpl();
		  WolConcatValueImplSptr concat_val = dConcatCast(subSplit);

		  // add stuff from _values and from subSplit
		  for (int i = 0; i < index; i++)
			  result->addValue(_values[i]);
		  for (auto it = concat_val->begin(); it != concat_val->end(); it++)
			  result->addValue(*it);
		  for (unsigned i = index + 1; i < _values.size(); i++)
			  result->addValue(_values[i]);

		  DEBUG4_MSG << "RESULT =" << result->getStringRep();
		  return result;
	  }
	  else {

		  WolUnionValueImplSptr result = makeUnionImpl();
		  WolUnionValueImplSptr union_val = dUnionCast(subSplit);

		  for (auto it = union_val->begin(); it != union_val->end(); it++) {

			  assert((*it)->getImplType() == WOL_VALUE_IMPL_CONCAT_TYPE);

			  WolConcatValueImplSptr tempResult = makeConcatImpl();
			  WolConcatValueImplSptr concat_val = dConcatCast(*it);

			  // add stuff from _values and *it
			  for (int i = 0; i < index; i++)
				  tempResult->addValue(_values[i]);
			  for (auto it1 = concat_val->begin(); it1 != concat_val->end(); it1++)
				  tempResult->addValue(*it1);
			  for (unsigned  i = index + 1; i < _values.size(); i++)
				  tempResult->addValue(_values[i]);

			  result->addValue(tempResult);
		  }

		  DEBUG4_MSG << "RESULT =" << result->getStringRep();
		  return result;
	  }

	  assert(0);
	  return nullptr;
  }


  WolValueImplSptr
  WolConcatValueImpl::getSlices(int upper, int lower) {
  //TODO: need for optimizations

    upper = _prec - upper - 1;
    lower = _prec - lower - 1;
    WolConcatValueImplSptr retValue = makeConcatImpl();
    int tempPrecision = 0;
    for(WolValueImplSptr i : _values) {

      WolValueImplSptr temp = nullptr;
      int tempNextPrecision = tempPrecision + i->getPrecision() - 1;

      if (tempPrecision > upper) break;

      if ((tempPrecision < lower) && (tempNextPrecision > upper))
	  return i->getSlices(lower-tempPrecision, upper-tempPrecision);

      if ((tempPrecision < lower) && (tempNextPrecision > lower) && (tempNextPrecision <= upper))
          temp = i->getSlices(lower - tempPrecision, i->getPrecision());

      if ((tempPrecision >= lower) && (tempNextPrecision <= upper))
	  temp = i;

      if ((tempPrecision >= lower) && (tempNextPrecision > upper))
	  temp = i->getSlices(0, upper - tempPrecision);

       if (temp)
           retValue->addValue(temp);
       tempPrecision += i->getPrecision();

    }

     if (retValue->numValues() == 1)
        return *retValue->begin();
     else
        return retValue;
  }

  dbitset
  WolConcatValueImpl::getLowValue() {

    dbitset result (_prec);
    int index = 0;
    for (auto i:_values) {

      dbitset temp = i->getLowValue();
      for (unsigned i = 0; i < temp.size(); i++) {
	result[index] = temp[i];
	index++;
      }

    }
    return result;
  }

  dbitset
  WolConcatValueImpl::getHighValue() {

    dbitset result (_prec);
    int index = 0;
    for (auto i:_values) {

      dbitset temp = i->getHighValue();
      for (unsigned i = 0; i < temp.size(); i++) {
	result[index] = temp[i];
	index++;
      }

    }
    return result;
  }

  WolValueImplSptr
  WolConcatValueImpl::getNotValueInt() {

	  DEBUG4_MSG << "CONCAT " << this->getStringRep()
			     << " NOT" ;
    WolConcatValueImplSptr retValue = makeConcatImpl();
    for (auto i :_values) {
      retValue->addValue(i->getNotValueInt());
    }

    DEBUG4_MSG << "RESULT =" << retValue->getStringRep();
    return retValue;
  }

  bool
  WolConcatValueImpl::containsValue(dbitset const_val) {

    int low_precision = 0;
    int high_precision = -1;
    for(auto i :_values) {
      high_precision += i->getPrecision();
      dbitset temp = createSubsetOfDynamicBitSet(const_val,low_precision,high_precision);
      if (!i->containsValue(temp)) return false;
      low_precision += i->getPrecision();
    }

    return true;
  }

  WolValueSptr
  WolConcatValueImpl::getRandomValue() {
    int size = _values.size();
    WolEvalFactory *evalFactory = WolMgr::getInstance().getEvalFactory();
    WolValueSptr retValue = _values[0]->getRandomValue();

    for (int i = 1; i < size; i++) {
      WolValueSptr temp = _values[i]->getRandomValue();
      retValue = evalFactory->evalConcat(retValue, temp);
    }

    return retValue;

  }


}
