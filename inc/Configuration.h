#ifndef __TCONFIGURATION_H__
#define __TCONFIGURATION_H__

#include <string>

// ---------------------------------------------------------------------------

#ifndef ___CONFIGURATION_CLASS
	#define ___CONFIGURATION_CLASS NRuevit::TConfiguration
#endif

#ifndef ___SECTION_CLASS
	#define ___SECTION_CLASS NRuevit::TSection
#endif

#ifndef ___PROPERTY_CLASS
	#define ___PROPERTY_CLASS NRuevit::TProperty
#endif

// ---------------------------------------------------------------------------

// Определя началото на конфигурацията
#ifdef IMPLEMENT_CONFIGURATION
#define CONFIGURATION(_NAME,_FILENAME) \
	namespace _NAME \
	{ \
		___CONFIGURATION_CLASS ____ConfigurationBegin ( #_NAME, _FILENAME );
#else
#define CONFIGURATION(_NAME,_FILENAME) \
	namespace _NAME \
	{ \
		extern ___CONFIGURATION_CLASS ____ConfigurationBegin;
#endif

// Определя края на конфигурацията
#ifdef IMPLEMENT_CONFIGURATION
#define END_CONFIGURATION \
		NRuevit::TConfigurationEnd ____ConfigurationEnd; \
		void Save ( void ) { NRuevit::TConfiguration::Configuration->Save(); } \
		void Load ( void ) { NRuevit::TConfiguration::Configuration->Load(); } \
	}
#else
#define END_CONFIGURATION \
		extern NRuevit::TConfigurationEnd ____ConfigurationEnd; \
		void Save ( void ); \
		void Load ( void ); \
	}
#endif

// Задава начало на секция с посоченото име
#ifdef IMPLEMENT_CONFIGURATION
#define SECTION(_NAME) \
	namespace _NAME \
	{ \
		___SECTION_CLASS ____SectionBegin ( #_NAME );
#else
#define SECTION(_NAME) \
	namespace _NAME \
	{ \
		extern ___SECTION_CLASS ____SectionBegin;
#endif

// Задава крой на последната започната секция
#ifdef IMPLEMENT_CONFIGURATION
#define END_SECTION \
		NRuevit::TSectionEnd ____SectionEnd; \
	}
#else
#define END_SECTION \
		extern NRuevit::TSectionEnd ____SectionEnd; \
	}
#endif

// Задава свойство
#ifdef IMPLEMENT_CONFIGURATION
#define PROPERTY(_TYPE,_NAME,_DEFVAL) \
	___PROPERTY_CLASS _NAME ( #_NAME, _TYPE(_DEFVAL) );
#else
#define PROPERTY(_TYPE,_NAME,_DEFVAL) \
	extern ___PROPERTY_CLASS _NAME;
#endif

// ---------------------------------------------------------------------------

namespace NRuevit
{

// ---------------------------------------------------------------------------

using std::string;

// ---------------------------------------------------------------------------

class TProperty
{
	friend class TSection;
	friend class TConfiguration;

	private:
		string Name, Value;
		TProperty *Next = 0;
		bool Changed = false;

		void Link ( void );

	public:
		template < typename T > TProperty ( const string &_Name, const T &_Value ) :
			Name ( _Name )
		{
			Link();
		}

		// Задаване на нова стойност на свойството
		template < typename T > TProperty& operator = ( const T &_Value )
		{
			Value = "";
			return *this;
		}

		// Конвертиране на свойството до основния тип данни
		template < typename T > operator T() const
		{
			return T();
		}

		// Връща специален текст за определени стойтости на параметъра
		// Може да се използва при изброими типове за запис на четими стойност
		template < typename T > string GetString ( const T &_Value ) const
		{
			return "";
		}

};

// ---------------------------------------------------------------------------

template<> TProperty::TProperty ( const string &_Name, const int &_Value );
template<> TProperty& TProperty::operator = ( const int &_Value );
template<> TProperty::operator int() const;

template<> TProperty::TProperty ( const string &_Name, const bool &_Value );
template<> TProperty& TProperty::operator = ( const bool &_Value );
template<> TProperty::operator bool() const;

template<> TProperty::TProperty ( const string &_Name, const double &_Value );
template<> TProperty& TProperty::operator = ( const double &_Value );
template<> TProperty::operator double() const;

template<> TProperty::TProperty ( const string &_Name, const string &_Value );
template<> TProperty& TProperty::operator = ( const string &_Value );
template<> TProperty::operator string() const;

// ---------------------------------------------------------------------------

class TSection
{
	friend class TProperty;
	friend class TSectionEnd;

	private:
		string Name;
		TSection *Next = 0, *Childs = 0, *Parent = 0;
		TProperty *Properties = 0;

		bool IsChanged ( void ) const;
		void Append ( TSection *_Section );

	public:
		static TSection *Current;

		explicit TSection ( const string &_Name );
		void Print ( void );
};

// ---------------------------------------------------------------------------

#define interface struct

struct IRunnable
{
	virtual void Run ( void ) = 0;
};

struct IChangeable
{
	virtual bool IsChanged ( void ) const = 0;
};

struct IConfiguration : public IChangeable
{
	enum EAutosaveIntervals
	{
		AUTOSAVE_ON_EXIT = -1,
		AUTOSAVE_DISABLED = 0,
		AUTOSAVE_ON_EVERY_CHANGE = 1,
		AUTOSAVE_OPTIMIZED = 5,
		AUTOSAVE_DELAYED = 10,
		AUTOSAVE_LONG_DELAYED = 100,
		AUTOSAVE_ON_5_CHANGES = 5,
		AUTOSAVE_ON_10_CHANGES = 10,
		AUTOSAVE_ON_100_CHANGES = 100
	};

	virtual void Load ( void ) = 0;
	virtual void Save ( void ) = 0;

	virtual void Load ( const std::string &_FileName ) = 0;
	virtual void Save ( const std::string &_FileName ) = 0;

	virtual void SetAutosaveInterval ( int _AutosaveMode ) = 0;
	virtual int GetAutosaveInterval ( void ) const = 0;
};

struct IConfigurationParser
{
	virtual void Parse ( void ) = 0;
}

class TConfiguration : public IConfiguration, TSection
{
	private:
		std::string FileName;
		int AutosaveInterval, CurrentAutosaveIntervalCounter;

	public:
		static TConfiguration *Configuration;

		explicit TConfiguration ( void ); // _Name = appname, _FileName = appname.cfg.xml
		explicit TConfiguration ( const std::string &_Name, const std::string &_FileName );
		virtual ~TConfiguration ( void );

		virtual void Load ( void );
		virtual void Save ( void );
};

// ---------------------------------------------------------------------------

struct TSectionEnd { TSectionEnd ( void ); };

// ---------------------------------------------------------------------------

struct TConfigurationEnd : public TSectionEnd { TConfigurationEnd ( void ); };

// ---------------------------------------------------------------------------

} //~ namespace NRuevit

#endif
