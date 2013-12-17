#pragma once

class Land {
    public:
        Land(const std::string& filename) {
            std::ifstream in(filename.c_str());
            if(in.good()) {
                std::string line;
                while(getline(in, line)) {
                    std::vector<bool> v;
                    std::transform(line.begin(), line.end(), back_inserter(v), is_land);
                    data.push_back(v);
                }
                m_height = data.size();
                if(!data.empty())
                    m_width = data.at(0).size();
            } else {
                std::cerr << "Error opening input file " << filename << std::endl;
            }
        }
        void walk_and_mark(int i, int j)
        {
            if(check_range(i, j) && is_set(i, j)) {
                reset(i, j);
                walk_and_mark(i-1, j);
                walk_and_mark(i, j+1);
                walk_and_mark(i+1, j);
                walk_and_mark(i, j-1);
            }
        }
        bool is_set(const int i, const int j) const { return at(i, j); }
        int width() const { return m_width; }
        int heigth() const { return m_height; }
        friend std::ostream& operator<<(std::ostream& os, const Land& l) {
            os << std::endl;
            for(int i = 0; i < l.m_height; i++) {
                for(int j = 0; j < l.m_width; j++) {
                    os << (l.is_set(i, j) ? "o" : "~");
                }
                os << std::endl;
            }
            return os;
        }
    private:
        std::vector<std::vector<bool> > data;
        int m_width;
        int m_height;
        bool check_range(const int i, const int j) const
        {
            return i >= 0 && j >= 0 && i < m_height && j < m_width;
        }
        bool at(const int i, const int j) const { return data.at(i).at(j); }
        static bool is_land(char c) { return (c == 'o'); }
        void reset(const int i, const int j) { data.at(i).at(j) = false; }
        Land(const Land&);        
        Land& operator=(Land&);
};