#include "TCanvas.h"
#include "TPad.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TAxis.h"
#include "TH1.h"
#include "TGraph.h"

#include <functional>

template<typename T>
class member {
  public:
    member(T data, std::function<void(void)> callback) :
        data(data), callback(callback) { }

    member& operator=(const T& value) {
        data = value;
        callback();

        return *this;
    }

    operator T() const { return data; }

  private:
    T data;
    std::function<void(void)> callback;
};

class tiling {
  public:
    tiling() :
        columns(1, std::bind(&tiling::tile, this)),
        rows(1, std::bind(&tiling::tile, this)),
        frame_width(400, std::bind(&tiling::tile, this)),
        frame_height(400, std::bind(&tiling::tile, this)),
        lmargin(0.2, std::bind(&tiling::tile, this)),
        rmargin(0.1, std::bind(&tiling::tile, this)),
        tmargin(0.1, std::bind(&tiling::tile, this)),
        bmargin(0.2, std::bind(&tiling::tile, this))
    { tile(); }
    tiling(int columns, int rows) :
        columns(columns, std::bind(&tiling::tile, this)),
        rows(rows, std::bind(&tiling::tile, this)),
        frame_width(400, std::bind(&tiling::tile, this)),
        frame_height(400, std::bind(&tiling::tile, this)),
        lmargin(0.2, std::bind(&tiling::tile, this)),
        rmargin(0.1, std::bind(&tiling::tile, this)),
        tmargin(0.1, std::bind(&tiling::tile, this)),
        bmargin(0.2, std::bind(&tiling::tile, this))
    { tile(); }
    tiling(int columns, int rows,
           float lmargin, float rmargin,
           float tmargin, float bmargin) :
        columns(columns, std::bind(&tiling::tile, this)),
        rows(rows, std::bind(&tiling::tile, this)),
        frame_width(400, std::bind(&tiling::tile, this)),
        frame_height(400, std::bind(&tiling::tile, this)),
        lmargin(lmargin, std::bind(&tiling::tile, this)),
        rmargin(rmargin, std::bind(&tiling::tile, this)),
        tmargin(tmargin, std::bind(&tiling::tile, this)),
        bmargin(bmargin, std::bind(&tiling::tile, this))
    { tile(); }
    tiling(int columns, int rows, int frame_width, int frame_height,
           float lmargin, float rmargin,
           float tmargin, float bmargin) :
        columns(columns, std::bind(&tiling::tile, this)),
        rows(rows, std::bind(&tiling::tile, this)),
        frame_width(frame_width, std::bind(&tiling::tile, this)),
        frame_height(frame_height, std::bind(&tiling::tile, this)),
        lmargin(lmargin, std::bind(&tiling::tile, this)),
        rmargin(rmargin, std::bind(&tiling::tile, this)),
        tmargin(tmargin, std::bind(&tiling::tile, this)),
        bmargin(bmargin, std::bind(&tiling::tile, this))
    { tile(); }

    member<int> columns, rows;
    member<int> frame_width, frame_height;
    member<float> lmargin, rmargin, tmargin, bmargin;

    float get_canvas_margin_left() { return canvas_margin_left; }
    float get_canvas_margin_right() { return canvas_margin_right; }
    float get_canvas_margin_top() { return canvas_margin_top; }
    float get_canvas_margin_bottom() { return canvas_margin_bottom; }

    /* convenience functions for common ROOT objects */
    TCanvas* create_canvas(const char* name, const char* title);
    TLegend* create_legend_on_frame(float x_min, float y_min,
                                    float x_max, float y_max,
                                    int font, float font_size,
                                    int col, int row);
    TLatex* draw_latex_on_canvas(float x, float y, const char* text,
                                   int font, float font_size, int align);
    TLatex* draw_latex_on_frame(float x, float y, const char* text,
                                  int font, float font_size, int align,
                                  int col, int row);

    void set_sizes(TH1* h1, int font,
                   float x_title_size, float y_title_size,
                   float x_label_size, float y_label_size,
                   float x_tick_size, float y_tick_size,
                   float x_title_offset, float y_title_offset,
                   float x_label_offset, float y_label_offset,
                   int col, int row);
    void set_sizes(TGraph* g1, int font,
                   float x_title_size, float y_title_size,
                   float x_label_size, float y_label_size,
                   float x_tick_size, float y_tick_size,
                   float x_title_offset, float y_title_offset,
                   float x_label_offset, float y_label_offset,
                   int col, int row);

    void cover_axis_labels(int x_options, int y_options);

  private:
    float lmargin_rel, rmargin_rel, tmargin_rel, bmargin_rel;

    float canvas_width, canvas_height;
    float canvas_margin_left, canvas_margin_right;
    float canvas_margin_top, canvas_margin_bottom;

    float canvas_frame_ratio_horiz, canvas_frame_ratio_vert;

    std::vector<float> tile_edges_horiz, tile_edges_vert;
    std::vector<float> frame_edges_horiz, frame_edges_vert;
    std::vector<float> tile_widths, tile_heights;

    void tile();

    void adjust_coordinates(float& x_min, float& y_min,
                            float& x_max, float& y_max,
                            int col, int row);

    inline float adjust_size(float size, int col, int row) {
        float charheight = size * std::min(tile_widths[0], tile_heights[0]);
        return charheight / std::min(tile_widths[col], tile_heights[row]); }
    inline float adjust_size_inverse(float size, int col, int row) {
        float charheight = size * std::min(tile_widths[col], tile_heights[row]);
        return charheight / std::min(tile_widths[0], tile_heights[0]); }
    inline float normalize_canvas_size(float size) {
        return size * frame_width / std::min(canvas_width, canvas_height); }
    inline float normalize_tile_size(float size) {
        return size * frame_width / std::min(tile_widths[0], tile_heights[0]); }
    inline float normalize_tile_size_inverse(float size) {
        return size * std::min(tile_widths[0], tile_heights[0]) / frame_width; }
};

void tiling::tile() {
    canvas_frame_ratio_horiz = columns + lmargin + rmargin;
    canvas_margin_left = lmargin / canvas_frame_ratio_horiz;
    canvas_margin_right = rmargin / canvas_frame_ratio_horiz;

    canvas_frame_ratio_vert = rows + tmargin + bmargin;
    canvas_margin_top = tmargin / canvas_frame_ratio_vert;
    canvas_margin_bottom = bmargin / canvas_frame_ratio_vert;

    if (columns > 1) {
        lmargin_rel = lmargin / (lmargin + 1.0);
        rmargin_rel = rmargin / (rmargin + 1.0);
    } else {
        lmargin_rel = lmargin / canvas_frame_ratio_horiz;
        rmargin_rel = rmargin / canvas_frame_ratio_horiz;
    }

    if (rows > 1) {
        tmargin_rel = tmargin / (tmargin + 1.0);
        bmargin_rel = bmargin / (bmargin + 1.0);
    } else {
        tmargin_rel = tmargin / canvas_frame_ratio_vert;
        bmargin_rel = bmargin / canvas_frame_ratio_vert;
    }

    // if (columns > 1) {
    //     canvas_frame_ratio_horiz = 1.0 / (1.0 - lmargin_rel) +
    //         1.0 / (1.0 - rmargin_rel) + columns - 2;
    //     canvas_margin_left = lmargin_rel / (1.0 - lmargin_rel) /
    //         canvas_frame_ratio_horiz;
    //     canvas_margin_right = rmargin_rel / (1.0 - rmargin_rel) /
    //         canvas_frame_ratio_horiz;
    // } else {
    //     canvas_frame_ratio_horiz = 1.0 / (1.0 - lmargin_rel - rmargin_rel);
    //     canvas_margin_left = lmargin_rel;
    //     canvas_margin_right = rmargin_rel;
    // }

    // if (rows > 1) {
    //     canvas_frame_ratio_vert = 1.0 / (1.0 - tmargin_rel) +
    //         1.0 / (1.0 - bmargin_rel) + rows - 2;
    //     canvas_margin_top = tmargin_rel / (1.0 - tmargin_rel) /
    //         canvas_frame_ratio_vert;
    //     canvas_margin_bottom = bmargin_rel / (1.0 - bmargin_rel) /
    //         canvas_frame_ratio_vert;
    // } else {
    //     canvas_frame_ratio_vert = 1.0 / (1.0 - tmargin_rel - bmargin_rel);
    //     canvas_margin_top = tmargin_rel;
    //     canvas_margin_bottom = bmargin_rel;
    // }

    canvas_width = frame_width * canvas_frame_ratio_horiz;
    canvas_height = frame_height * canvas_frame_ratio_vert;

    tile_edges_horiz.clear();
    tile_edges_horiz.push_back(0.0);
    tile_edges_horiz.push_back(canvas_margin_left / lmargin_rel);
    for (int i=2; i<columns; ++i) {
        tile_edges_horiz.push_back(
            tile_edges_horiz[i-1] + 1.0 / canvas_frame_ratio_horiz
        );
    }
    if (columns > 1) { tile_edges_horiz.push_back(1.0); }

    frame_edges_horiz.clear();
    tile_widths.clear();

    frame_edges_horiz.push_back(canvas_margin_left);
    for (int i=0; i<columns; ++i) {
        frame_edges_horiz.push_back(
            frame_edges_horiz[i] + 1.0 / canvas_frame_ratio_horiz
        );
        tile_widths.push_back(
            canvas_width *
            (tile_edges_horiz[i+1] - tile_edges_horiz[i])
        );
    }

    tile_edges_vert.clear();
    tile_edges_vert.push_back(1.0);
    tile_edges_vert.push_back(1.0 - canvas_margin_top / tmargin_rel);
    for (int i=2; i<rows; ++i) {
        tile_edges_vert.push_back(
            tile_edges_vert[i-1] - 1.0 / canvas_frame_ratio_vert
        );
    }
    if (rows > 1) { tile_edges_vert.push_back(0.0); }

    frame_edges_vert.clear();
    tile_heights.clear();

    frame_edges_vert.push_back(1.0 - canvas_margin_top);
    for (int i=0; i<rows; ++i) {
        frame_edges_vert.push_back(
            frame_edges_vert[i] - 1.0 / canvas_frame_ratio_vert
        );
        tile_heights.push_back(
            canvas_height * (tile_edges_vert[i] - tile_edges_vert[i+1])
        );
    }
}

TCanvas* tiling::create_canvas(const char* name, const char* title) {
    TCanvas* c1 = new TCanvas(name, title, canvas_width, canvas_height);

    c1->Clear();

    /* help the compiler understand that member<int> can be converted to int */
    TPad* tiles[rows + 0][columns + 0];
    for (int i=0; i<rows; ++i) {
        for (int j=0; j<columns; ++j) {
            c1->cd();

            tiles[i][j] = new TPad(
                Form("tile_%i_%i", i, j), "",
                tile_edges_horiz[j], tile_edges_vert[i+1],
                tile_edges_horiz[j+1], tile_edges_vert[i]
            );

            if (i == 0) tiles[i][j]->SetTopMargin(tmargin_rel);
            else        tiles[i][j]->SetTopMargin(0);
            if (i == rows - 1) tiles[i][j]->SetBottomMargin(bmargin_rel);
            else               tiles[i][j]->SetBottomMargin(0);
            if (j == 0) tiles[i][j]->SetLeftMargin(lmargin_rel);
            else        tiles[i][j]->SetLeftMargin(0);
            if (j == columns - 1) tiles[i][j]->SetRightMargin(rmargin_rel);
            else                  tiles[i][j]->SetRightMargin(0);

            tiles[i][j]->Draw();
            tiles[i][j]->cd();
            tiles[i][j]->SetNumber(i * columns + j + 1);
            tiles[i][j]->SetTickx();
            tiles[i][j]->SetTicky();
        }
    }

    return c1;
}

TLegend* tiling::create_legend_on_frame(float x_min, float y_min,
                                        float x_max, float y_max,
                                        int font, float font_size,
                                        int col, int row) {
    adjust_coordinates(x_min, y_min, x_max, y_max, col, row);
    TLegend* l1 = new TLegend(x_min, y_min, x_max, y_max);

    l1->SetBorderSize(0);
    l1->SetFillStyle(0);

    l1->SetTextFont(font * 10 + 2);
    l1->SetTextSize(normalize_tile_size(adjust_size(font_size, col, row)));

    return l1;
}

TLatex* tiling::draw_latex_on_canvas(float x, float y, const char* text,
                                     int font, float font_size, int align) {
    float norm_size = normalize_canvas_size(font_size);

    TLatex* l1 = new TLatex();
    l1->SetTextFont(font * 10 + 2);
    l1->SetTextSize(norm_size);
    l1->SetTextAlign(align);

    return l1->DrawLatexNDC(x, y + norm_size * 0.8, text);
}

TLatex* tiling::draw_latex_on_frame(float x, float y, const char* text,
                                    int font, float font_size, int align,
                                    int col, int row) {
    float x_tmp = 1;
    float y_tmp = 1;
    adjust_coordinates(x, y, x_tmp, y_tmp, col, row);

    TLatex* l1 = new TLatex();
    l1->SetTextFont(font * 10 + 2);
    l1->SetTextSize(normalize_tile_size(adjust_size(font_size, col, row)));
    l1->SetTextAlign(align);

    return l1->DrawLatexNDC(x, y, text);
}

void tiling::set_sizes(TH1* h1, int font,
                       float x_title_size, float y_title_size,
                       float x_label_size, float y_label_size,
                       float x_tick_size, float y_tick_size,
                       float x_title_offset, float y_title_offset,
                       float x_label_offset, float y_label_offset,
                       int col, int row) {
    font = font * 10 + 2;

    y_title_size = adjust_size(y_title_size, col, row);
    y_label_size = adjust_size(y_label_size, col, row);
    y_tick_size = y_tick_size * tile_heights[row] /frame_height *
        tile_widths[0] / tile_widths[col];
    y_title_offset = adjust_size_inverse(y_title_offset, col, row);

    y_title_size = normalize_tile_size(y_title_size);
    y_label_size = normalize_tile_size(y_label_size);
    y_tick_size = normalize_tile_size(y_tick_size);
    y_title_offset = normalize_tile_size_inverse(y_title_offset);

    x_title_size = adjust_size(x_title_size, col, row);
    x_label_size = adjust_size(x_label_size, col, row);
    x_tick_size = x_tick_size * tile_widths[col] / frame_width *
        tile_widths[0] / tile_heights[0];
    x_title_offset = adjust_size_inverse(x_title_offset, col, row);
    x_label_offset = x_label_offset - x_label_size * 0.8;

    x_title_size = normalize_tile_size(x_title_size);
    x_label_size = normalize_tile_size(x_label_size);
    x_tick_size = normalize_tile_size(x_tick_size);
    x_label_offset = normalize_tile_size(x_label_offset);

    TAxis* x_axis = h1->GetXaxis();
    TAxis* y_axis = h1->GetYaxis();

    x_axis->SetTitleFont(font);
    x_axis->SetTitleSize(x_title_size);
    x_axis->SetLabelFont(font);
    x_axis->SetLabelSize(x_label_size);
    x_axis->SetLabelOffset(x_label_offset);
    x_axis->SetTickSize(x_tick_size);

    y_axis->SetTitleFont(font);
    y_axis->SetTitleSize(y_title_size);
    y_axis->SetLabelFont(font);
    y_axis->SetLabelSize(y_label_size);
    y_axis->SetLabelOffset(y_label_offset);
    y_axis->SetTickSize(y_tick_size);

    if (row == rows - 1) {
        x_axis->SetTitleOffset(x_title_offset);
        x_axis->CenterTitle();
    } else {
        x_axis->SetTitle("");
        x_axis->SetTitleOffset(999);
        x_axis->SetLabelOffset(999);
    }

    if (col == 0) {
        y_axis->SetTitleOffset(y_title_offset);
        y_axis->CenterTitle();
    } else {
        y_axis->SetTitle("");
        y_axis->SetTitleOffset(999);
        y_axis->SetLabelOffset(999);
    }
}

void tiling::set_sizes(TGraph* g1, int font,
                       float x_title_size, float y_title_size,
                       float x_label_size, float y_label_size,
                       float x_tick_size, float y_tick_size,
                       float x_title_offset, float y_title_offset,
                       float x_label_offset, float y_label_offset,
                       int col, int row) {
    font = font * 10 + 2;

    y_title_size = adjust_size(y_title_size, col, row);
    y_label_size = adjust_size(y_label_size, col, row);
    y_tick_size = y_tick_size * tile_heights[row] /frame_height *
        tile_widths[0] / tile_widths[col];
    y_title_offset = adjust_size_inverse(y_title_offset, col, row);

    y_title_size = normalize_tile_size(y_title_size);
    y_label_size = normalize_tile_size(y_label_size);
    y_tick_size = normalize_tile_size(y_tick_size);
    y_title_offset = normalize_tile_size_inverse(y_title_offset);

    x_title_size = adjust_size(x_title_size, col, row);
    x_label_size = adjust_size(x_label_size, col, row);
    x_tick_size = x_tick_size * tile_widths[col] / frame_width *
        tile_widths[0] / tile_heights[0];
    x_title_offset = adjust_size_inverse(x_title_offset, col, row);
    x_label_offset = x_label_offset - x_label_size * 0.8;

    x_title_size = normalize_tile_size(x_title_size);
    x_label_size = normalize_tile_size(x_label_size);
    x_tick_size = normalize_tile_size(x_tick_size);
    x_label_offset = normalize_tile_size(x_label_offset);

    TAxis* x_axis = g1->GetXaxis();
    TAxis* y_axis = g1->GetYaxis();

    x_axis->SetTitleFont(font);
    x_axis->SetTitleSize(x_title_size);
    x_axis->SetLabelFont(font);
    x_axis->SetLabelSize(x_label_size);
    x_axis->SetLabelOffset(x_label_offset);
    x_axis->SetTickSize(x_tick_size);

    y_axis->SetTitleFont(font);
    y_axis->SetTitleSize(y_title_size);
    y_axis->SetLabelFont(font);
    y_axis->SetLabelSize(y_label_size);
    y_axis->SetLabelOffset(y_label_offset);
    y_axis->SetTickSize(y_tick_size);

    if (row == rows - 1) {
        x_axis->SetTitleOffset(x_title_offset);
        x_axis->CenterTitle();
    } else {
        x_axis->SetTitle("");
        x_axis->SetTitleOffset(999);
        x_axis->SetLabelOffset(999);
    }

    if (col == 0) {
        y_axis->SetTitleOffset(y_title_offset);
        y_axis->CenterTitle();
    } else {
        y_axis->SetTitle("");
        y_axis->SetTitleOffset(999);
        y_axis->SetLabelOffset(999);
    }
}

/* x_options(y_options):
 *
 * for each panel:
 * 0: don't cover
 * 1: cover left(top) side only
 * 2: cover right(bottom) side only
 * 3: cover both sides
 *
 * options apply starting from the right(bottom)-most panel
 *
 * covers up to the edge of the panel, except when the panel is the
 * left(top)- or right(bottom)-most one
 *
 * warning! leading zeros indicate an octal number (c language feature)
 *
 * e.g. for a plot with 5 columns:
 *      33333 covers everything,
 *      23331 covers everything except the left-most and right-most label
 *      3333 covers everything in the last 4 frames
 */
void tiling::cover_axis_labels(int x_options, int y_options) {
    float x_min, x_max;
    float y_min, y_max;

    for (int i = columns - 1; i >= 0 && x_options != 0; --i, x_options /= 10) {
        int opt = x_options % 10;

        y_min = frame_edges_vert[rows] - 0.05;
        y_max = y_min + 0.048;

        if (opt & 0x1) {
            x_min = frame_edges_horiz[i] - 0.0005;
            x_max = x_min + 0.0125;

            if (i == 0) { x_min -= 0.015; }

            TPad* cover = new TPad(
                Form("x_cover_left_%i", x_options),
                Form("x_cover_left_%i", x_options),
                x_min, y_min, x_max, y_max
            );
            cover->Draw();
        }

        if (opt & 0x2) {
            x_min = frame_edges_horiz[i + 1] - 0.012;
            x_max = x_min + 0.0125;

            if (i == columns - 1) { x_max += 0.015; }

            TPad* cover = new TPad(
                Form("x_cover_right_%i", x_options),
                Form("x_cover_right_%i", x_options),
                x_min, y_min, x_max, y_max
            );
            cover->Draw();
        }
    }

    for (int i = rows - 1; i >= 0 && y_options != 0; --i, y_options /= 10) {
        int opt = y_options % 10;

        x_min = frame_edges_horiz[0] - 0.03;
        x_max = x_min + 0.029;

        if (opt & 0x1) {
            y_min = frame_edges_vert[i] - 0.02;
            y_max = y_min + 0.0205;

            if (i == 0) { y_max += 0.04; }

            TPad* cover = new TPad(
                Form("x_cover_top_%i", y_options),
                Form("x_cover_top_%i", y_options),
                x_min, y_min, x_max, y_max
            );
            cover->Draw();
        }

        if (opt & 0x2) {
            y_min = frame_edges_vert[i + 1] - 0.0005;
            y_max = y_min + 0.0205;

            if (i == rows - 1) { y_min -= 0.04; }

            TPad* cover = new TPad(
                Form("x_cover_bottom_%i", y_options),
                Form("x_cover_bottom_%i", y_options),
                x_min, y_min, x_max, y_max
            );
            cover->Draw();
        }
    }
}

void tiling::adjust_coordinates(float& x_min, float& y_min,
                                float& x_max, float& y_max,
                                int col, int row) {
    if (columns > 1) {
        if (col == 0) {
            x_min = x_min * (1.0 - lmargin_rel) + lmargin_rel;
            x_max = x_max * (1.0 - lmargin_rel) + lmargin_rel;
        } else if (col == columns - 1) {
            x_min = x_min * (1.0 - rmargin_rel);
            x_max = x_max * (1.0 - rmargin_rel);
        }
    } else {
        x_min = x_min * (1.0 - lmargin_rel - rmargin_rel) + lmargin_rel;
        x_max = x_max * (1.0 - lmargin_rel - rmargin_rel) + lmargin_rel;
    }

    if (rows > 1) {
        if (row == 0) {
            y_min = y_min * (1.0 - tmargin_rel);
            y_max = y_max * (1.0 - tmargin_rel);
        } else if (row == rows - 1) {
            y_min = y_min * (1.0 - bmargin_rel) + bmargin_rel;
            y_max = y_max * (1.0 - bmargin_rel) + bmargin_rel;
        }
    } else {
        y_min = y_min * (1.0 - tmargin_rel - bmargin_rel) + bmargin_rel;
        y_max = y_max * (1.0 - tmargin_rel - bmargin_rel) + bmargin_rel;
    }
}
