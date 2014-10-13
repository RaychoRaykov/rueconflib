#include "rueconflib.hpp"

namespace NRuevit
{

// ---------------------------------------------------------------------------

const char *TRUE_STRING_VALUE = "TRUE";
const char *FALSE_STRING_VALUE = "FALSE";

// ---------------------------------------------------------------------------

void TPropertyBase::Link ( void )
{
	Section = TSection::Current;
	TProperty **p = &Section->Properties;
	while ( *p ) p = &(*p)->Next;
	*p = this;
}

// ---------------------------------------------------------------------------

void TPropertyBase::Change ( void )
{
	ChangedFlag = true;
	Section->Change();
}

// ---------------------------------------------------------------------------

TPropertyBase::TPropertyBase ( const std::string &_Name ) : Name ( _Name )
{
	Link();
}

// ---------------------------------------------------------------------------

template<> TProperty::TProperty ( const string &_Name, const int &_Value ) :
	Name ( _Name )
{
	Value = std::to_string ( _Value );
	Link();
}

template<> TProperty& TProperty::operator = ( const int &_Value )
{
	Value = std::to_string ( _Value );
	Changed = true;
	return *this;
}

template<> TProperty::operator int() const
{
	return std::stoi ( Value );
}

// ---------------------------------------------------------------------------

template<> TProperty::TProperty ( const string &_Name, const bool &_Value ) :
	Name ( _Name )
{
	Value = _Value ? TRUE_STRING_VALUE : FALSE_STRING_VALUE;
	Link();
}

template<> TProperty& TProperty::operator = ( const bool &_Value )
{
	Value = _Value ? TRUE_STRING_VALUE : FALSE_STRING_VALUE;
	Changed = true;
	return *this;
}

template<> TProperty::operator bool() const
{
	return Value == TRUE_STRING_VALUE;
}

// ---------------------------------------------------------------------------

template<> TProperty::TProperty ( const string &_Name, const double &_Value ) :
	Name ( _Name )
{
	Value = std::to_string ( _Value );
	Link();
}

template<> TProperty& TProperty::operator = ( const double &_Value )
{
	Value = std::to_string ( _Value );
	Changed = true;
	return *this;
}

template<> TProperty::operator double() const
{
	return std::stod ( Value );
}

// ---------------------------------------------------------------------------

template<> TProperty::TProperty ( const string &_Name, const string &_Value ) :
	Name ( _Name )
{
	Value = _Value;
	Link();
}

template<> TProperty& TProperty::operator = ( const string &_Value )
{
	Value = _Value;
	Changed = true;
	return *this;
}

template<> TProperty::operator string() const
{
	return Value;
}

// ---------------------------------------------------------------------------

TSection::TSection ( const string &_Name ) : Name ( _Name )
{
	if ( Current )
	{
		Parent = Current;
		if ( Current->Childs ) Current->Childs->Append ( this );
		else Current->Childs = this;
	}
	Current = this;
}

void TSection::Append ( TSection *_Section )
{
	if ( Next ) Next->Append ( _Section );
	else Next = _Section;
}

void TSection::Change ( void )
{
	Changed = true;
	if ( Parent ) Parent->Change();
}

bool TSection::IsChanged ( void ) const
{
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
{
	if ( !Configuration ) Configuration = this;
}

TConfiguration::~TConfiguration ( void )
{
	Save();
}

void TConfiguration::Load ( void )
{
	Print();
}

void TConfiguration::Save ( void )
{
	Print();
}

// ---------------------------------------------------------------------------

TSectionEnd::TSectionEnd ( void )
{
	TSection::Current = TSection::Current->Parent;
}

// ---------------------------------------------------------------------------


TConfigurationEnd::TConfigurationEnd ( void )
{
	TConfiguration::Configuration->Load();
}

// ---------------------------------------------------------------------------

} //~ NRuevit
