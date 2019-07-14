#include "Recipe.h"

bool Recipe::matchesSearchTerm(const QString& term) const
{
	//search recipe name
	if (name.contains(term, Qt::CaseInsensitive)) return true;

	//search text
	foreach(const QSharedPointer<RecipePart>& part, parts)
	{
		if (part->matchesSearchTerm(term)) return true;
	}

	return false;
}

bool Recipe::matchesSearchTerms(const QStringList& terms) const
{
	foreach(const QString& term, terms)
	{
		if (!matchesSearchTerm(term)) return false;
	}

	return true;
}

void Recipe::toHTML(QTextStream& stream, int recipe_nr) const
{
	//recipe header
	stream << "  <a href='javascript:toggle(" << recipe_nr << ");' style='color: darkred; text-decoration: underline;font-size: 1.1em;' >" << name << "</a>\n";
	stream << "  <div id='toggle" << recipe_nr << "' style='display: none' >\n";
	stream << "  <br>\n";

	//amount
	stream << "  <font>(" << amount << ")</font>\n";
	stream << "  <br>\n";

	//content
	stream << "  <table>";
	foreach(const QSharedPointer<RecipePart>& part, parts)
	{
		part->toHTML(stream);
	}
	stream << "  </table>\n";
	stream << "  </div>\n";
}

void Recipe::toXML(QTextStream& stream) const
{
	stream << "\t<recipe name=\"" << name << "\" amount=\"" << amount << "\" type=\"" << type << "\">\n";

	foreach(const QSharedPointer<RecipePart>& part, parts)
	{
		part->toXML(stream);
	}

	stream << "\t</recipe>\n\n";
}

RecipeSection::~RecipeSection()
{
}

void RecipeSection::toHTML(QTextStream& stream) const
{
	//section
	stream << "  <tr>\n";
	stream << "    <td colspan='3'> <b>" << text << "</b> </td>\n";
	stream << "  </tr>\n";
}

void RecipeSection::toXML(QTextStream& stream) const
{
	stream << "\t\t<section>" << text << "</section>\n";
}

bool RecipeSection::matchesSearchTerm(const QString& term) const
{
	return text.contains(term, Qt::CaseInsensitive);
}

RecipeIngredient::~RecipeIngredient()
{
}

void RecipeIngredient::toHTML(QTextStream& stream) const
{
	stream << "  <tr>\n";
	stream << "    <td valign='top'><nobr>" << amount << " " << unit  << "</nobr></td>\n";
	stream << "    <td valign='top'><nobr>" << name  << "</nobr></td>\n";
	stream << "    <td valign='top'>" << text  << "</td>\n";
	stream << "  </tr>\n";
}

void RecipeIngredient::toXML(QTextStream& stream) const
{
	stream << "\t\t<ingr name=\"" << name << "\"";
	if (!amount.isEmpty()) stream << " amount=\"" << amount << "\"";
	if (!unit.isEmpty()) stream << " unit=\"" << unit << "\"";
	stream << ">" << text << "</ingr>\n";
}

bool RecipeIngredient::matchesSearchTerm(const QString& term) const
{
	return name.contains(term, Qt::CaseInsensitive) || text.contains(term, Qt::CaseInsensitive);
}

RecipePart::~RecipePart()
{
}

RecipeText::~RecipeText()
{
}

void RecipeText::toHTML(QTextStream& stream) const
{
	stream << "  <tr>\n";
	stream << "    <td valign='top'></td>\n";
	stream << "    <td valign='top'></td>\n";
	stream << "    <td valign='top'>" << text  << "</td>\n";
	stream << "  </tr>\n";
}

void RecipeText::toXML(QTextStream& stream) const
{
	stream << "\t\t<text>" << text << "</text>\n";
}

bool RecipeText::matchesSearchTerm(const QString& term) const
{
	return text.contains(term, Qt::CaseInsensitive);
}
