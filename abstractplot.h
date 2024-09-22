#pragma once

#include <QWidget>

#include <any>

template<class T>
concept Plot = requires(
    T Object,
    const std::vector<std::vector<double>> data1,
    const std::vector<double>& data2,
    QVector<double> &&data3,
    unsigned int nColumns,
    const std::unordered_map<unsigned int, std::any> &parameters
)
{
    { Object.addData(data1) };
    { Object.addData(data2) };
    { Object.setData(std::move(data3), nColumns) };
    { Object.setParameters(parameters) };
    { Object.start() };
};

template<Plot T>
class AbstractPlot
{

public:
    explicit AbstractPlot(QWidget *parent = nullptr) : _impl(std::make_unique<T>(parent)) {};

    inline void addData(const std::vector<double> &data) { this->_impl->addData(data); }
    inline QWidget *const instance() const { return this->_impl.get(); }
    inline void setData(QVector<double> &&data, const unsigned int nColumns) { this->_impl->setData(std::move(data), nColumns); }
    inline void setParameters(const std::unordered_map<unsigned int, std::any> &parameters) { this->_impl->setParameters(parameters); }
    inline void start() { this->_impl->start(); }

private:
    std::unique_ptr<T> _impl;

};
