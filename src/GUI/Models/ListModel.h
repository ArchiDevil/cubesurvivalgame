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


    class ListModel
    {
    public:
        static const size_t npos = (size_t)-1;

        ListModel(IModelSubscriber * parent);
        void Add(const std::string &element, size_t pos = ListModel::npos);

        const std::string & Get(size_t index) const;
        size_t Get(const std::string &element) const;
        size_t Count() const;

        void Remove(const std::string &element);
        void Remove(size_t pos);
        void Clear();

    private:
        IModelSubscriber * parent;
        std::vector<std::string> elements;

    };

}
