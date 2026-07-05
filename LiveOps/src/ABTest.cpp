// ABTest.cpp
#include "ABTest.h"
namespace PolygonalPrimordials::LiveOps {
    void ABTest::create(const Experiment&) {}
    void ABTest::start(const std::string&) {}
    void ABTest::stop(const std::string&) {}
    ExperimentVariant ABTest::assign(const std::string&, const std::string&) { return ExperimentVariant::Control; }
    ExperimentAssignment ABTest::get_assignment(const std::string&, const std::string&) const { return {}; }
    void ABTest::record(const std::string&, ExperimentVariant, const std::string&, double) {}
    ExperimentResult ABTest::analyze(const std::string&) const { return {}; }
    std::vector<Experiment> ABTest::list() const { return {}; }
    std::vector<Experiment> ABTest::active_experiments() const { return {}; }
    ExperimentVariant ABTest::weighted_choice(const Experiment&, std::mt19937&) const { return ExperimentVariant::Control; }
    double ABTest::compute_p_value(const std::vector<double>&, const std::vector<double>&) const { return 0; }
}
