#ifndef RECIPE_H
#define RECIPE_H

#include <QString>
#include <QList>
#include <QTextStream>
#include <QSharedPointer>

//Base class for recipe parts
class RecipePart
{
public:
	virtual ~RecipePart();
	virtual bool matchesSearchTerm(const QString& term) const = 0;
	virtual void toHTML(QTextStream& stream) const = 0;
};

class RecipeIngredient
	: public RecipePart
{
public:
	virtual ~RecipeIngredient();
	bool matchesSearchTerm(const QString& term) const override;
	void toHTML(QTextStream& stream) const override;

	QString name;
	QString amount;
	QString unit;
	QString text;
};

class RecipeSection
	: public RecipePart
{
public:
	virtual ~RecipeSection();
	bool matchesSearchTerm(const QString& term) const override;
	void toHTML(QTextStream& stream) const override;

	QString text;
};

class RecipeText
	: public RecipePart
{
public:
	virtual ~RecipeText();
	bool matchesSearchTerm(const QString& term) const override;
	void toHTML(QTextStream& stream) const override;

	QString text;
};

class Recipe
{
public:
	bool matchesSearchTerm(const QString& term) const;
	bool matchesSearchTerms(const QStringList& terms) const;
	void toHTML(QTextStream& stream, int recipe_nr=-1) const;

	QString name;
	QString amount;
	QString type;

	QList<QSharedPointer<RecipePart>> parts;
};

#endif // RECIPE_H
