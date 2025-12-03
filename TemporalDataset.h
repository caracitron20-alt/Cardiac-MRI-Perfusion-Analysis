// Base Class for handling time-series image datasets
#pragma once

#include <vector>
#include <string>
#include <format>

#include "image.h"
#include "debug.h"


template <typename ValueType>
class TemporalDataset
{
  public:
    // Contructors
    TemporalDataset () = default;
    TemporalDataset (const std::vector<std::string>& filenames) { load (filenames); }

    // Virtual destructor for inheritance
    virtual ~TemporalDataset()=default;
    
    // Load a sequence of iages from PGM files
    void load (const std::vector<std::string>& filenames);

    // Get size of the dataset
    unsigned int size () const { return m_slices.size(); }

    // Operators
    Image<ValueType>&       operator[] (int n)       { return m_slices[n]; }
    const Image<ValueType>& operator[] (int n) const { return m_slices[n]; }


  protected:
    std::vector<Image<ValueType>> m_slices; 
};

// Implementation of load method
template <typename ValueType>
void TemporalDataset<ValueType>::load (const std::vector<std::string>& filenames)
{
  m_slices.clear();
  // Check input
  if (filenames.empty())
    throw std::runtime_error ("no filenames supplied when loading dataset");

  // Load all files into m_slices
  for (const auto& fname : filenames)
    m_slices.push_back (load_pgm (fname));

  // Check that dimensions all match up:
  for (unsigned int n = 1; n < m_slices.size(); ++n) {
    if ( (m_slices[n].width() != m_slices[n-1].width()) ||
         (m_slices[n].height() != m_slices[n-1].height()) )
      throw std::runtime_error ("dimensions do not match across slices");
  }

  debug::log (std::format (
      "loaded {} slices of size {}x{}\n",
      m_slices.size(), m_slices[0].width(), m_slices[0].height()));
}

