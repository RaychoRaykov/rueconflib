#include <stdio.h>

#include "logging.hpp"

#include "Configuration.h"

namespace NRuevit
{

// ---------------------------------------------------------------------------

const char *TRUE_STRING_VALUE = "TRUE";
const char *FALSE_STRING_VALUE = "FALSE";

// ---------------------------------------------------------------------------

void TProperty::Link ( void )
{
	TProperty **p = &TSection::Current->Properties;
	while ( *p ) p = &(*p)->Next;
	*p = this;
}

template<> TProperty::TProperty ( const string &_Name, const int &_Value ) :
	Name ( _Name )
{LOG
	Value = std::to_string ( _Value );
	Link();
}

template<> TProperty& TProperty::operator = ( const int &_Value )
{LOG
	Value = std::to_string ( _Value );
	Changed = true;
	return *this;
}

template<> TProperty::operator int() const
{LOG
	return std::stoi ( Value );
}

// ---------------------------------------------------------------------------

template<> TProperty::TProperty ( const string &_Name, const bool &_Value ) :
	Name ( _Name )
{LOG
	Value = _Value ? TRUE_STRING_VALUE : FALSE_STRING_VALUE;
	Link();
}

template<> TProperty& TProperty::operator = ( const bool &_Value )
{LOG
	Value = _Value ? TRUE_STRING_VALUE : FALSE_STRING_VALUE;
	Changed = true;
	return *this;
}

template<> TProperty::operator bool() const
{LOG
	return Value == TRUE_STRING_VALUE;
}

// ---------------------------------------------------------------------------

template<> TProperty::TProperty ( const string &_Name, const double &_Value ) :
	Name ( _Name )
{LOG
	Value = std::to_string ( _Value );
	Link();
}

template<> TProperty& TProperty::operator = ( const double &_Value )
{LOG
	Value = std::to_string ( _Value );
	Changed = true;
	return *this;
}

template<> TProperty::operator double() const
{LOG
	return std::stod ( Value );
}

// ---------------------------------------------------------------------------

template<> TProperty::TProperty ( const string &_Name, const string &_Value ) :
	Name ( _Name )
{LOG
	Value = _Value;
	Link();
}

template<> TProperty& TProperty::operator = ( const string &_Value )
{LOG
	Value = _Value;
	Changed = true;
	return *this;
}

template<> TProperty::operator string() const
{LOG
	return Value;
}

// ---------------------------------------------------------------------------

TSection::TSection ( const string &_Name ) : Name ( _Name )
{LOG
	if ( Current )
	{
		Parent = Current;
		if ( Current->Childs ) Current->Childs->Append ( this );
		else Current->Childs = this;
	}
	Current = this;
}

void TSection::Append ( TSection *_Section )
{LOG
	if ( Next ) Next->Append ( _Section );
	else Next = _Section;
}

bool TSection::IsChanged ( void ) const
{LOG
	return false;
}

void Indent ( int _Indent )
{
	while ( _Indent-- ) putchar ( '\t' );
}

void TSection::Print ( void )
{
	static int indent = 0;
	putchar ( '\n' );
	Indent ( indent );
	printf ( "[%s]", Name.c_str() );
	indent++;
	if ( Childs ) Childs->Print();
	TProperty *p = Properties;
	while ( p )
	{
		putchar ( '\n' );
		Indent ( indent );
		printf ( "%s = %s", p->Name.c_str(), p->Value.c_str() );
		p = p->Next;
	}
	indent--;
	if ( Next ) Next->Print();
}

// ---------------------------------------------------------------------------

TSection *TSection::Current = 0;

// ---------------------------------------------------------------------------

TConfiguration* TConfiguration::Configuration = 0;

TConfiguration::TConfiguration ( const string &_Name, const string &_FileName ) :
	TSection ( _Name ), FileName ( _FileName )
{LOG
	if ( !Configuration ) Configuration = this;
}

TConfiguration::~TConfiguration ( void )
{LOG
	Save();
}

void TConfiguration::Load ( void )
{LOG
	Print();
}

void TConfiguration::Save ( void )
{LOG
	Print();
}

// ---------------------------------------------------------------------------

TSectionEnd::TSectionEnd ( void )
{LOG
	TSection::Current = TSection::Current->Parent;
}

// ---------------------------------------------------------------------------


TConfigurationEnd::TConfigurationEnd ( void )
{LOG
	TConfiguration::Configuration->Load();
}

// ---------------------------------------------------------------------------

} //~ NRuevit
