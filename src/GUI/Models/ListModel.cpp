#include "ListModel.h"

SimpleGUI::IModelSubscriber::IModelSubscriber(ListModel * model)
    : model(model)
{
}

//////////////////////////////////////////////////////////////////////////

SimpleGUI::ListModel::ListModel(IModelSubscriber * parent)
    : parent(parent)
{
}

void SimpleGUI::ListModel::Add(const std::string &element, size_t pos /*= ListModel::npos*/)
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

const std::string & SimpleGUI::ListModel::Get(size_t index) const
{
    if (index < elements.size())
        return elements[index];

    static std::string s = "";
    return s;
}

size_t SimpleGUI::ListModel::Get(const std::string &element) const
{
    auto iter = std::find(elements.cbegin(), elements.cend(), element);
    if (iter != elements.end())
        return std::distance(iter, elements.cbegin());

    return npos;
}

void SimpleGUI::ListModel::Remove(const std::string &element)
{
    auto iter = std::find(elements.cbegin(), elements.cend(), element);
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
