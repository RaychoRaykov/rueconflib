#ifndef __RUECONFLIB_HPP__
#define __RUECONFLIB_HPP__

#include <string>

// ---------------------------------------------------------------------------

#ifndef ___RUECONFLIB_CONFIGURATION_CLASS
	#define ___RUECONFLIB_CONFIGURATION_CLASS NRuevit::TConfiguration
#endif

#ifndef ___RUECONFLIB_SECTION_CLASS
	#define ___RUECONFLIB_SECTION_CLASS NRuevit::TSection
#endif

#ifndef ___RUECONFLIB_PROPERTY_CLASS
	#define ___RUECONFLIB_PROPERTY_CLASS NRuevit::TProperty
#endif

// ---------------------------------------------------------------------------

// Определя началото на конфигурацията
#ifdef RUECONFLIB_IMPLEMENT_CONFIGURATION
#define RUECONFLIB_CONFIGURATION(_NAME,_FILENAME) \
	namespace _NAME \
	{ \
		___RUECONFLIB_CONFIGURATION_CLASS ____ConfigurationBegin ( #_NAME, _FILENAME );
#else
#define RUECONFLIB_CONFIGURATION(_NAME,_FILENAME) \
	namespace _NAME \
	{ \
		extern ___RUECONFLIB_CONFIGURATION_CLASS ____ConfigurationBegin;
#endif

// Определя края на конфигурацията
#ifdef RUECONFLIB_IMPLEMENT_CONFIGURATION
#define RUECONFLIB_END_CONFIGURATION \
		NRuevit::TConfigurationEnd ____ConfigurationEnd; \
		void Save ( void ) { NRuevit::TConfiguration::Configuration->Save(); } \
		void Load ( void ) { NRuevit::TConfiguration::Configuration->Load(); } \
	}
#else
#define RUECONFLIB_END_CONFIGURATION \
		extern NRuevit::TConfigurationEnd ____ConfigurationEnd; \
		void Save ( void ); \
		void Load ( void ); \
	}
#endif

// Задава начало на секция с посоченото име
#ifdef RUECONFLIB_IMPLEMENT_CONFIGURATION
#define RUECONFLIB_SECTION(_NAME) \
	namespace _NAME \
	{ \
		___RUECONFLIB_SECTION_CLASS ____SectionBegin ( #_NAME );
#else
#define SECTION(_NAME) \
	namespace _NAME \
	{ \
		extern ___RUECONFLIB_SECTION_CLASS ____SectionBegin;
#endif

// Задава крой на последната започната секция
#ifdef RUECONFLIB_IMPLEMENT_CONFIGURATION
#define RUECONFLIB_END_SECTION \
		NRuevit::TSectionEnd ____SectionEnd; \
	}
#else
#define RUECONFLIB_END_SECTION \
		extern NRuevit::TSectionEnd ____SectionEnd; \
	}
#endif

// Задава свойство
#ifdef RUECONFLIB_IMPLEMENT_CONFIGURATION
#define RUECONFLIB_PROPERTY(_TYPE,_NAME,_DEFVAL) \
	___RUECONFLIB_PROPERTY_CLASS _NAME ( #_NAME, _TYPE(_DEFVAL) );
#else
#define RUECONFLIB_PROPERTY(_TYPE,_NAME,_DEFVAL) \
	extern ___RUECONFLIB_PROPERTY_CLASS _NAME;
#endif

// ---------------------------------------------------------------------------

#ifndef CONFIGURATION
	#define CONFIGURATION RUECONFLIB_CONFIGURATION
#endif

#ifndef SECTION 
	#define SECTION RUECONFLIB_SECTION
#endif

#ifndef PROPERTY
	#define PROPERTY RUECONFLIB_PROPERTY
#endif

#ifndef IMPLEMENT_CONFIGURATION
	#define IMPLEMENT_CONFIGURATION RUECONFLIB_CONFIGURATION
#endif

// ---------------------------------------------------------------------------

namespace NRueConfLib
{

// ---------------------------------------------------------------------------

class TPropertyBase
{
	friend class TSection;
	friend class TConfiguration;
	
	protected:
	
		std::string Name, Description;
		TProperty *Next = nullptr;
		TSection *Section = nullptr;
		bool NullFlag = true;
		bool ChangedFlag = false;
		
		/*************************************************************************
		 * Добавя свойството към списъка от свойства на избраната секция.
		 * Добавя указател към секцията, която съдържа свойството.
		 */
		void Link ( void );
		
		/*************************************************************************
		 * Вдига флага за промяна на свойството, както и флага за промяна 
		 * на секцията, която го съдържа, чрез извикване на нейния метод
		 * Change()
		 */
		void Change ( void );

	public:
	
		/*************************************************************************
		 * Връща описание на свойството. По принцип това описание трябва да може
		 * да се запазва във файла и да се извлича от него. По този начин то
		 * ще може да се променя в него.
		 */
		inline std::string GetDescription ( void ) const
		{
			return Description;
		}

		/*************************************************************************
		 * Проверява дали стойността на свойството е NULL, т.е. няма зададена
		 * стойност.
		 */
		inline bool IsNull ( void ) const 
		{
			return NullFlag;
		}

		/*************************************************************************
		 * Задава стойност NULL на свойството. Това може да се проверява чрез
		 * метода IsNull().
		 */
		inline void SetToNull ( void )
		{
			if ( !NullFlag )
			{
				NullFlag = true;
				Change();
			}
		}

		explicit TPropertyBase ( const std::string &_Name );
		
};

template < typename TValueType >
class TProperty : public TPropertyBase
{
	friend class TSection;
	friend class TConfiguration;

	private:

		TValueType Value;
	
		inline TProperty ( const std::string &_Name ) : TPropertyBase ( _Name )
		{
		}
	
		TProperty ( const string &_Name, const TValueType &_Value ) :
			TPropertyBase ( _Name ), Value ( _Value )
		{
			Link();
			NullFlag = false;
		}

		// Задаване на нова стойност на свойството
		TProperty& operator = ( const TValueType &_Value )
		{
			if ( !( Value == _Value ) )
			{
				Value = _Value;
				NullFlag = false;
				Change();
			}
			return *this;
		}

		// Конвертиране на свойството до основния тип данни
		operator TValueType() const
		{
			return Value;
		}

};

// ---------------------------------------------------------------------------

class TSection
{
	friend class TPropertyBase;
	friend class TSectionEnd;

	private:
		std::string Name;
		TSection *Next = 0, *Childs = 0, *Parent = 0;
		TProperty *Properties = 0;

		void Append ( TSection *_Section );
		void Change ( void );
		
		static TSection *Current;

	public:
		bool IsChanged ( void ) const;
		explicit TSection ( const std::string &_Name );
		void Print ( void );
};

// ---------------------------------------------------------------------------

struct IConfigurationFile
{
	virtual void Load ( void ) = 0;
	virtual void Save ( void ) = 0;
}

struct IConfiguration
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
