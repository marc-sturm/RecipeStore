#include "Recipe.h"

void Recipe::toHTML(QTextStream& stream) const
{
	//recipe header
	stream << "  <h4>" << name << "</h4>\n";
	stream << "  <br>\n";

	//amount
	stream << "  <font>(" << amount << ")</font>\n";
	stream << "  <br>\n";
	stream << "  <br>\n";

	//content
	stream << "  <table>";
	foreach(const RecipePart& part, parts)
	{
		part.toHTML(stream);
	}
	foreach(const RecipeIngredient& ingr, ingredients)
	{
		ingr.toHTML(stream);
	}
	stream << "  </table>\n";
}

void RecipePart::toHTML(QTextStream& stream) const
{
	//section
	stream << "  <tr>\n";
	stream << "    <td colspan='3'> <b>" << name << "</b> </td>\n";
	stream << "  </tr>\n";

	//content
	foreach(const RecipeIngredient& ingr, ingredients)
	{
		ingr.toHTML(stream);
	}
}

void RecipeIngredient::toHTML(QTextStream& stream) const
{
	stream << "  <tr>\n";
	stream << "    <td valign='top'><nobr>" << amount << " " << unit  << "</nobr></td>\n";
	stream << "    <td valign='top'><nobr>" << name  << "</nobr></td>\n";
	stream << "    <td valign='top'>TODO</td>\n";
	stream << "  </tr>\n";
}
