#include "Recipe.h"

void Recipe::toHTML(QTextStream& stream) const
{
	//recipe header
	stream << "  <h3>" << name << "</h3>\n";
	stream << "  <br>\n";

	//amount
	stream << "  <font>(" << amount << ")</font>\n";
	stream << "  <br>\n";
	stream << "  <br>\n";

	//content
	stream << "  <table>";
	foreach(const RecipePart* part, parts)
	{
		part->toHTML(stream);
	}
	stream << "  </table>\n";
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
