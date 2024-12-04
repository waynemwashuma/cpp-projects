#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <math.h>

int year_index = 2;
int month_index = 3;
int date_index = 4;
int exposure_index = 5;

struct Information
{
  std::string location;
  std::string log;
  size_t columns;
  size_t rows;
  std::string highest_date;
  std::string lowest_date;
  float mean;
  float median;
  float mode;
  int mode_no;
};

void bubble_sort(std::vector<float> &v)
{
  int n = v.size();
  for (int i = 0; i < n - 1; i++)
  {
    for (int j = 0; j < n - i - 1; j++)
    {
      if (v[j] > v[j + 1])
      {
        float temp = v[j];
        v[j] = v[j + 1];
        v[j + 1] = temp;
      }
    }
  }
}

// Splits a string using a given separator.
std::vector<std::string> split_string(std::string str, char separator)
{
  std::vector<std::string> strings;
  int startIndex = 0, endIndex = 0;

  for (int i = 0; i <= str.size(); i++)
  {
    if (str[i] == separator || i == str.size())
    {
      std::string temp;
      endIndex = i;
      temp.append(str, startIndex, endIndex - startIndex);
      strings.push_back(temp);
      // plus one to skip the separator
      startIndex = endIndex + 1;
    }
  }
  return strings;
}

// converts internal file format into a single string to save to disk.
std::string convert_to_file_data(std::vector<std::vector<std::string>> file)
{
  std::string data;
  for (int i = 0; i < file.size(); i++)
  {
    std::vector<std::string> row = file.at(i);
    for (int i = 0; i < row.size(); i++)
    {
      data += row.at(i);
      // place commas except for last place.
      if (i != row.size() - 1)
      {
        data += ',';
      }
    }
    data += '\n';
  }

  return data;
}

void cleanup_csv_data(std::vector<std::vector<std::string>> &data)
{
  for (int i = data.size() - 1; i >= 0; i--)
  {
    bool erase_row = false;

    for (int j = 0; j < data[i].size(); j++)
    {
      if (data[i][j] == "")
      {
        erase_row = true;
        break;
      }
    }
    if (erase_row)
    {
      data.erase(data.begin() + i);
    }
  }
}

// Loads the csv file from disk
std::vector<std::vector<std::string>> load_csv_file(std::string file_path)
{
  std::fstream file;
  std::vector<std::vector<std::string>> parts;
  file.open(file_path);
  if (!file.is_open())
  {
    std::cout << "Failed to open file: " << file_path << "\n";
  }
  std::string line;
  while (std::getline(file, line))
  {
    parts.push_back(split_string(line, ','));
  }

  file.close();
  cleanup_csv_data(parts);
  return parts;
}

// Converts units from MJ/m^2 to KWh/m^2
float convert_solar_exposure(float value)
{
  // 1MJ = 0.2777777778KWh
  return value * 0.2777777778;
}

std::vector<std::vector<std::string>> convert_file_units(std::vector<std::vector<std::string>> file)
{
  for (int i = 1; i < file.size(); i++)
  {
    std::vector<std::string> row = file.at(i);
    float exposure = convert_solar_exposure(std::stof(row[exposure_index]));
    file.at(i)[exposure_index] = std::to_string(exposure);
  }
  return file;
}

std::string highest_exposure(std::vector<std::vector<std::string>> file, int start, int length)
{
  int exposure_index = 5;
  float exposure = 0;
  std::string date;
  for (int i = start; i < length; i++)
  {
    if (std::stof(file.at(i).at(exposure_index)) > exposure)
    {
      exposure = std::stof(file.at(i).at(exposure_index));
      date = file.at(i).at(date_index) + '/' + file.at(i).at(month_index) + '/' + file.at(i).at(year_index);
    }
  }

  return date;
}

std::string lowest_exposure(std::vector<std::vector<std::string>> file, int start, int length)
{
  int exposure_index = 5;
  float exposure = 10000000;
  std::string date;
  for (int i = start; i < length; i++)
  {
    if (std::stof(file.at(i).at(exposure_index)) < exposure)
    {
      exposure = std::stof(file.at(i).at(exposure_index));
      date = file.at(i).at(date_index) + '/' + file.at(i).at(month_index) + '/' + file.at(i).at(year_index);
    }
  }

  return date;
}

float mean_exposure(std::vector<std::vector<std::string>> file, int start, int length)
{
  float exposure = 0;
  for (int i = start; i < length; i++)
  {
    exposure += std::stof(file.at(i).at(exposure_index));
  }
  return exposure / (file.size() - 1);
}

float median_exposure(std::vector<std::vector<std::string>> file, int start, int length)
{
  std::vector<float> exposures;
  for (int i = start; i < length; i++)
  {
    exposures.push_back(std::stof(file.at(i).at(exposure_index)));
  }
  bubble_sort(exposures);
  if (exposures.size() % 2 == 0)
  {
    return exposures.at((exposures.size() / 2));
  }
  else
  {
    return exposures.at(exposures.size() / 2);
  }
}

float mode_exposure(std::vector<std::vector<std::string>> file)
{
  std::vector<float> exposure;
  for (int i = 1; i < file.size(); i++)
  {
    exposure.push_back(std::stof(file.at(i).at(exposure_index)));
  }
  bubble_sort(exposure);

  int number = exposure[0];
  int mode = number;
  int count = 1;
  int countMode = 1;

  for (int i = 1; i < exposure.size(); i++)
  {
    if (exposure[i] == number)
    {
      ++count;
    }
    else
    {
      if (count > countMode)
      {
        countMode = count;
        mode = number;
      }
      count = 1;
      number = exposure[i];
    }
  }
  return mode;
}

int mode_exposure_number(std::vector<std::vector<std::string>> file)
{
  std::vector<float> exposure;
  for (int i = 1; i < file.size(); i++)
  {
    exposure.push_back(std::stof(file.at(i).at(exposure_index)));
  }
  bubble_sort(exposure);

  int number = exposure[0];
  int mode = number;
  int count = 1;
  int countMode = 1;

  for (int i = 1; i < exposure.size(); i++)
  {
    if (exposure[i] == number)
    {
      ++count;
    }
    else
    {
      if (count > countMode)
      {
        countMode = count;
        mode = number;
      }
      count = 1;
      number = exposure[i];
    }
  }
  return countMode;
}

float variance_exposure(std::vector<std::vector<std::string>> file, int start, int length)
{
  float min = 1000000, max = 0;
  for (int i = start; i < length; i++)
  {
    float value = std::stof(file.at(i).at(exposure_index));
    if (value < min)
      min = value;
    if (value > max)
      max = value;
  }

  return max - min;
}

void save_file(std::string file_path, std::string data)
{
  std::ofstream file;

  file.open(file_path);
  if (!file.is_open())
  {
    std::cout << "Cannot write to file " + file_path + '\n';
  }
  file << data;
  file.close();
}

std::string highest_median_date(std::vector<std::vector<std::string>> file)
{
  float highest = 0;
  std::string highest_month;
  std::string highest_year;
  int row = 1;

  for (int i = 1; i < file.size(); i++)
  {
    std::string month = file.at(row).at(month_index);
    std::string next_month = file.at(i).at(month_index);
    if (next_month != month)
    {
      float median = median_exposure(file, row, i - 1);

      month = next_month;
      if (median > highest)
      {
        highest = median;
        highest_year = file.at(i).at(year_index);
        highest_month = month;
      }
      row = i;
    }
  }

  return highest_month + '/' + highest_year;
}

std::string lowest_median_date(std::vector<std::vector<std::string>> file)
{
  float lowest = 10000;
  std::string lowest_month;
  std::string lowest_year;
  int row = 1;

  for (int i = 1; i < file.size(); i++)
  {
    std::string month = file.at(row).at(month_index);
    std::string next_month = file.at(i).at(month_index);
    if (next_month != month)
    {
      float median = median_exposure(file, row, i - 1);
      month = next_month;
      if (median < lowest)
      {
        lowest = median;
        lowest_year = file.at(i).at(year_index);
        lowest_month = month;
      }
      row = i;
    }
  }

  return lowest_month + '/' + lowest_year;
}

std::string highest_mean_date(std::vector<std::vector<std::string>> file)
{
  float highest = 0;
  std::string highest_month;
  std::string highest_year;
  int row = 1;

  for (int i = 1; i < file.size(); i++)
  {
    std::string month = file.at(row).at(month_index);
    std::string next_month = file.at(i).at(month_index);
    if (next_month != month)
    {
      float median = mean_exposure(file, row, i - 1);
      month = next_month;
      if (median > highest)
      {
        highest = median;
        highest_year = file.at(i).at(year_index);
        highest_month = month;
      }
      row = i;
    }
  }

  float median = mean_exposure(file, row, file.size());

  if (median > highest)
  {
    highest = median;
    highest_year = file.at(row).at(year_index);
    highest_month = file.at(row).at(month_index);
  }

  return highest_month + '/' + highest_year;
}

std::string lowest_mean_date(std::vector<std::vector<std::string>> file)
{
  float lowest = 100000;
  std::string lowest_month;
  std::string lowest_year;
  int row = 1;

  for (int i = 1; i < file.size(); i++)
  {
    std::string month = file.at(row).at(month_index);
    std::string next_month = file.at(i).at(month_index);
    if (next_month != month)
    {
      float median = mean_exposure(file, row, i - 1);
      month = next_month;
      if (median < lowest)
      {
        lowest = median;
        lowest_year = file.at(i).at(year_index);
        lowest_month = month;
      }
      row = i;
    }
  }

  float median = mean_exposure(file, row, file.size());

  if (median < lowest)
  {
    lowest = median;
    lowest_year = file.at(row).at(year_index);
    lowest_month = file.at(row).at(month_index);
  }

  return lowest_month + '/' + lowest_year;
}

std::string least_variant_month(std::vector<std::vector<std::string>> file)
{
  float lowest = 100000;
  std::string lowest_month;
  std::string lowest_year;
  int row = 1;

  for (int i = 1; i < file.size(); i++)
  {
    std::string month = file.at(row).at(month_index);
    std::string next_month = file.at(i).at(month_index);
    if (next_month != month)
    {
      float median = variance_exposure(file, row, i - 1);
      month = next_month;
      if (median < lowest)
      {
        lowest = median;
        lowest_year = file.at(i).at(year_index);
        lowest_month = month;
      }
      row = i;
    }
  }

  float median = variance_exposure(file, row, file.size());

  if (median < lowest)
  {
    lowest = median;
    lowest_year = file.at(row).at(year_index);
    lowest_month = file.at(row).at(month_index);
  }

  return lowest_month + '/' + lowest_year;
}

Information do_everything_else(std::string name, std::vector<std::vector<std::string>> file)
{
  std::string information = "";
  // Display which dataset this is for
  std::string dataset_message = "----------\nThe following shows data for " + name + "\n-----------\n";
  std::cout << dataset_message;
  information += dataset_message;

  // Convert exposure units to KWh/m^2
  file = convert_file_units(file);
  save_file(name + "_KWhm^-2.csv", convert_to_file_data(file));

  // Display number of columns
  std::string column = "Number of columns: " + std::to_string(file.at(0).size()) + '\n';
  std::cout << column;
  information += column;

  // Display number of rows except the heading of csv.
  std::string row = "Number of rows: " + std::to_string(file.size() - 1) + '\n' + '\n';
  std::cout << row;
  information += row;

  // Highest exposure date
  std::string highest = "Date of highest exposure: " + highest_exposure(file, 1, file.size()) + '\n';
  std::cout << highest;
  information += highest;

  // Lowest exposure date
  std::string lowest = "Date of lowest exposure: " + lowest_exposure(file, 1, file.size()) + "\n";
  std::cout << lowest;
  information += lowest;

  // Mean exposure
  std::string mean = "Mean exposure: " + std::to_string(mean_exposure(file, 1, file.size())) + "KWH/m^2\n\n";
  std::cout << mean;
  information += mean;

  // Median exposure
  std::string median = "Median exposure: " + std::to_string(median_exposure(file, 1, file.size())) + "KWH/m^2\n";
  std::cout << median;
  information += median;

  // Mode and number of mode
  std::string mode = "Mode exposure: " + std::to_string(std::roundf(mode_exposure(file))) + "KWH/m^2\n" + "Number of mode exposure: " + std::to_string(mode_exposure_number(file)) + '\n';
  std::cout << mode;
  information += mode;

  // Highest mean date
  std::string highest_mean = "Highest mean month: " + highest_mean_date(file) + '\n';
  std::cout << highest_mean;
  information += highest_mean;

  // Lowest mean date
  std::string lowest_mean = "Lowest mean month: " + lowest_mean_date(file) + '\n';
  std::cout << lowest_mean;
  information += lowest_mean;

  // Highest median date
  std::string highest_median = "Highest median month: " + highest_median_date(file) + '\n';
  std::cout << highest_median;
  information += highest_median;

  // Lowest median date
  std::string lowest_median = "Lowest median month: " + lowest_median_date(file) + '\n';
  std::cout << lowest_median;
  information += lowest_median;

  // Least variant month
  std::string least_variant = "Least variant month: " + least_variant_month(file) + '\n';
  std::cout << least_variant;
  information += least_variant;

  // Displays calculations again
  std::string accept_show;
  std::cout << "Display the results again?Enter y for yes and any other for no:";
  std::cin >> accept_show;
  information += "Display the results again?Enter y for yes and any other for no:" + accept_show + '\n';

  if (accept_show == "y")
  {
    std::string name = column + row + highest + lowest + mean + median + mode + highest_mean + lowest_mean + highest_median + lowest_median + least_variant;
    std::cout << name;
    information += name;
  }

  // Prompts user if they would like to continue over and over
  // until user provides a valid response.
  while (true)
  {
    std::cout << "Would you like to continue?\nEnter 'c'/'C' to continue or 'e'/'E' to escape\n";
    std::string character;
    std::cin >> character;
    if (character == "c" || character == "C")
    {
      break;
    }
    if (character == "e" || character == "E")
    {
      return Information{
        information,
        log : information,
        columns : file.at(0).size(),
        rows : file.size(),
        highest_date : highest_exposure(file, 1, file.size()),
        lowest_date : lowest_exposure(file, 1, file.size()),
        mean : mean_exposure(file, 1, file.size()),
        median : median_exposure(file, 1, file.size()),
        mode : mode_exposure(file),
        mode_no : mode_exposure_number(file)
      };
    }
  }
  return Information{
    location : name,
    log : information,
    columns : file.at(0).size(),
    rows : file.size(),
    highest_date : highest_exposure(file, 1, file.size()),
    lowest_date : lowest_exposure(file, 1, file.size()),
    mean : mean_exposure(file, 1, file.size()),
    median : median_exposure(file, 1, file.size()),
    mode : mode_exposure(file),
    mode_no : mode_exposure_number(file)
  };
}

int main()
{
  std::vector<Information> info_group;
  std::string log;
  // prints out the welcoming message.
  std::string welcome = "[Redacted] Solar Exposure Analyser 1.0\n[Redacted]\n[Redacted]\n[Redacted]\nDesired level: [Redacted]\n\n";
  std::cout << welcome;
  log += welcome;

  // shows user how to use the program.
  std::string prompt = "This program is used to select an appropriate location\nfor a new solar installations.\nThe following locations are available:\n   avalon : loads data from Avalon.\n   caulfield : loads data from Caulfield Racecourse.\n   both : loads data from Avalon and Caulfield Racecourse.\nThe following year annotations can be used:\n   2023 : for 2023.\n   All_Years : For all years\n\n";
  std::cout << prompt;
  log += prompt;

  // Prompt user which location to take.
  std::string location, year;
  while (true)
  {
    std::string value = "Enter a location from above given locations(e.g both):";
    std::cout << value;
    std::cin >> location;

    log += value;
    log += location + '\n';

    if (location == "avalon" || location == "caulfeild" || location == "both")
    {
      break;
    }
    std::string error = "Invalid location!Please enter a valid location.\n";
    std::cout << error;
    log += error;
  }
  while (true)
  {
    std::string value = "Enter a year from above given locations(e.g 2023):";
    std::cout << value;
    std::cin >> year;

    log += value;
    log += year + '\n';

    if (year == "2023" || year == "All_Years")
    {
      break;
    }
    std::string error = "Invalid year!Please enter a valid year.\n";
    std::cout << error;
    log += error;
  }
  std::string avalon_raw = "Avalon_" + year + "_Data";
  std::string avalon = "data/" + avalon_raw + ".csv";
  std::string caulfield_raw = "Caulfeild_Racecourse_" + year + "_Data";
  std::string caulfield = "data/" + caulfield_raw + ".csv";
  // loads data from the csv file
  std::vector<std::vector<std::string>> file;
  if (location == "avalon")
  {
    file = load_csv_file(avalon);
    Information inf = do_everything_else(avalon_raw, file);
    inf.location = avalon;
    info_group.push_back(inf);
  }
  else if (location == "caulfeild")
  {
    Information inf = do_everything_else(caulfield_raw, file);
    file = load_csv_file(caulfield);
    inf.location = caulfield;
    info_group.push_back(inf);
  }
  else if (location == "both")
  {
    file = load_csv_file(avalon);
    Information inf1 = do_everything_else(avalon_raw, file);
    inf1.location = avalon;
    file = load_csv_file(caulfield);
    Information inf2 = do_everything_else(caulfield_raw, file);
    inf2.location = caulfield;
    info_group.push_back(inf1);
    info_group.push_back(inf2);
  }

  for (int i = 0; i < info_group.size(); i++)
  {
    log += info_group[i].log;
  }

  std::string data_rows = "file name,columns,rows,highest exposure date,lowest exposure date,mean exposure,median exposure,mode exposure,mode exposure count";

  for (int i = 0; i < info_group.size(); i++)
  {
    std::string columns = std::to_string(info_group[i].columns);
    std::string rows = std::to_string(info_group[i].rows);
    std::string highest_date = info_group[i].highest_date;
    std::string lowest_date = info_group[i].lowest_date;
    std::string mean = std::to_string(info_group[i].rows);
    std::string median = std::to_string(info_group[i].rows);
    std::string mode = std::to_string(info_group[i].rows);
    std::string mode_count = std::to_string(info_group[i].rows);

    data_rows.append(info_group[i].location + ',');
    data_rows.append(columns + ',');
    data_rows.append(rows + ',');
    data_rows.append(highest_date + ',');
    data_rows.append(lowest_date + ',');
    data_rows.append(mean + ',');
    data_rows.append(median + ',');
    data_rows.append(mode + ',');
    data_rows.append(mode_count);
  }

  // Logs out calculations
  save_file("Results/results.csv", data_rows);
  // Logs out information into log.txt
  save_file("Results/log.txt", log);
}
