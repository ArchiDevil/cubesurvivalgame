#include "ListModel.h"

SimpleGUI::IModelSubscriber::IModelSubscriber(ListModel * model)
    : model(model)
{
}

//////////////////////////////////////////////////////////////////////////

SimpleGUI::ListModelElement::ListModelElement()
    : name()
{
}

SimpleGUI::ListModelElement::ListModelElement(const std::string & name)
    : name(name)
{
}

const std::string & SimpleGUI::ListModelElement::GetName() const
{
    return name;
}

void SimpleGUI::ListModelElement::SetName(const std::string & val)
{
    name = val;
}

const std::string & SimpleGUI::ListModelElement::GetValue(const std::string & name) const
{
    static std::string emptyVal = "";

    auto iter = values.find(name);
    if (iter != values.end())
        return iter->second;

    return emptyVal;
}

void SimpleGUI::ListModelElement::SetValue(const std::string & name, const std::string & value)
{
    values[name] = value;
}

//////////////////////////////////////////////////////////////////////////

SimpleGUI::ListModel::ListModel(IModelSubscriber * parent)
    : parent(parent)
{
}

void SimpleGUI::ListModel::Add(const std::string &element, size_t pos /*= ListModel::npos*/)
{
    Add(ListModelElement(element), pos);
}

void SimpleGUI::ListModel::Add(const ListModelElement &element, size_t pos /*= ListModel::npos*/)
{
    if (pos >= elements.size()) // also npos
    {
        elements.push_back(element);
    }
    else
    {
        auto iter = elements.cbegin();
        for (size_t i = 0; i < pos; ++i, ++iter);
        elements.insert(iter, element);
    }

    if (parent)
        parent->OnModelChanged();
}

const SimpleGUI::ListModelElement & SimpleGUI::ListModel::Get(size_t index) const
{
    if (index < elements.size())
        return elements[index];

    static ListModelElement empty{};
    return empty;
}

size_t SimpleGUI::ListModel::Get(const std::string &name) const
{
    auto iter = std::find_if(elements.cbegin(), elements.cend(), [=](const ListModelElement &el) {return el.GetName() == name;});
    if (iter != elements.end())
        return std::distance(iter, elements.cbegin());

    return npos;
}

void SimpleGUI::ListModel::Remove(const std::string &name)
{
    auto iter = std::find_if(elements.cbegin(), elements.cend(), [=](const ListModelElement &el) {return el.GetName() == name;});
    if (iter != elements.end())
        elements.erase(iter);
}

void SimpleGUI::ListModel::Remove(size_t pos)
{
    if (pos >= elements.size())
        return;

    auto iter = elements.cbegin();
    for (size_t i = 0; i < pos; ++i, ++iter);
    elements.erase(iter);
}

void SimpleGUI::ListModel::Clear()
{
    elements.clear();
}

size_t SimpleGUI::ListModel::Count() const
{
    return elements.size();
}
