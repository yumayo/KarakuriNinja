#pragma once

//! @file Property
//! @brief Get Set���Ȃ������߂ɍ쐬���܂���
//! @note
//!        �l�b�g����E�������̂ł��B             
//! @date 2016-07-12
//! @author Taka Nomoto
namespace MySystem
{
	template <typename T>
	class Property
	{
	public:

		Property(T& value) : _value(value) {}

		Property(const Property& ref) : _value(ref._value) {}

		virtual ~Property() {}


		Property<T>& operator = (const Property<T>& ref)
		{
			this->_value = T(ref._value);
			return *this;
		}

	protected:
		//�g�p����l
		T&      _value;
	};

	template <typename T>
	struct SimplePropertyGetter
	{
		static const T& Get(const T& value) { return value; }
	};

	template <typename T>
	struct SimplePropertySetter
	{
		static void Set(T& value, const T& var) { value = T(var); }
	};

	template <typename T, class Getter = SimplePropertyGetter<T>, class Setter = SimplePropertySetter<T>>
	class WritableProperty : public Property<T>, private Getter, private Setter
	{
	public:
		WritableProperty(T& value) : Property<T>(value) {}
		WritableProperty(const WritableProperty& ref) : Property<T>(ref) {}
		virtual ~WritableProperty() {}

	public:
		operator const T& () const { return this->Get(this->_value); }
		const T& operator -> () const { return this->Get(this->_value); }

		WritableProperty<T, Getter, Setter>& operator = (const T& var) { this->Set(this->_value, var); return *this; }
	};
}