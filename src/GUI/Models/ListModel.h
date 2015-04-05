#pragma once

#include <string>

#include "../Objects/Base.h"

namespace SimpleGUI
{
    class ListModel;

    class IModelSubscriber
    {
    public:
        IModelSubscriber(ListModel * model);
        virtual ~IModelSubscriber() {}
        virtual void OnModelChanged() = 0;

    private:
        ListModel * model;
    };

    class ListModelElement
    {
    public:
        ListModelElement();
        ListModelElement(const std::string & name);

        const std::string & GetName() const;
        void SetName(const std::string & name);

        const std::string & GetValue(const std::string & name) const;
        void SetValue(const std::string & name, const std::string & value);

    private:
        std::string name;
        std::unordered_map<std::string, std::string> values;
    };

    class ListModel
    {
    public:
        static const size_t npos = (size_t)-1;

        ListModel(IModelSubscriber * parent);
        void Add(const std::string &element, size_t pos = ListModel::npos);
        void Add(const ListModelElement &element, size_t pos = ListModel::npos);

        const ListModelElement & Get(size_t index) const;
        size_t Get(const std::string &name) const;
        size_t Count() const;

        void Remove(const std::string &element);
        void Remove(size_t pos);
        void Clear();

    private:
        IModelSubscriber * parent;
        std::vector<ListModelElement> elements;

    };
}
