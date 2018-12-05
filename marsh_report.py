#!/usr/bin/python2.7
"""Generates MARSH reports based on raw output from MARSH benchmarks"""

class MarshTestReport(object):

    def __init__(self, name):
        self.name = name
        self.config = None
        self.passed = None
        self.accum = self.retry_warm = self.retry = None
        self.warmup = self.elapsed = None
        self.errors = self.iterations = None
        self.data_r = self.data_w = None
        self.npe = None
        self.mark = None
        self.bandwidth_r = self.bandwidth_w = None
        self.variant = None

    def parse_line(self, line):
        """Gets test report's lines and parse it"""
        if line.startswith("marsh_config="):
            self.config = line[len("marsh_config="):].strip()
            self.variant = self.config[:self.config.find(' ')]
            return
        if line.startswith("test passed"):
            self.passed = True
            return
        if line.startswith("test failed"):
            self.passed = False
            return
        for item in line.split():
            item_split = item.split('=')
            if len(item_split) != 2:
                continue
            (item_k, item_v) = item_split
            if item_k in set(['accum', 'errors', 'iterations']):
                # integer
                setattr(self, item_k, int(item_v))
                continue
            if item_k in set(['npe', 'mark']):
                # float
                setattr(self, item_k, float(item_v))
                continue
            if item_k in set(['warmup', 'elapsed']):
                # float, seconds
                setattr(self, item_k, float(item_v[:-1]))
                continue
            if item_k in set(['data[r/w]', 'bandwidth[r/w]']):
                # TODO: maybe add later
                # pair of floats
                continue
            if item_k in set(['retry']):
                # TODO: maybe add later
                # pair of integers
                continue
            print "WARNING: item=%s" % item

    def __str__(self):
        try:
            return '%s %s %s %.3f %.3f %.3f' % (
                self.name, str(self.passed), self.variant,
                self.npe, self.mark, self.elapsed
            )
        except:
            print 'ERROR: %s' % self.name
            return ''

class MarshReport(object):

    def __init__(self, textlist, freq=None):
        self.freq = freq
        self.test_reports = {}
        for line in textlist:
            tok = line.find(': ')
            if tok == -1:
                continue
            testname = line[:tok].strip()
            if testname not in self.test_reports:
                self.test_reports[testname] = MarshTestReport(testname)
            self.test_reports[testname].parse_line(line[tok+1:].strip())

    def __str__(self):
        rows = [str(self.test_reports[k]).split() for k in sorted(self.test_reports)]
        rows = [row for row in rows if row]
        header = ['name', 'status', 'variant', 'npe', 'mark', 'elapsed']
        if self.freq:
            header.append('cpe')
            rows = [row + ['%.3f' % (float(row[3]) * self.freq)] for row in rows]
        rows = [header] + rows
        widths = [0] * len(rows[0])
        for row in rows:
            widths = [w if w > len(s) else len(s) for s,w in zip(row, widths)]
        return '\n'.join(
            [' '.join([s.ljust(w, ' ') for (s,w) in zip(row, widths)]) for row in rows])

if __name__ == '__main__':
    # interactive use
    import argparse
    AP = argparse.ArgumentParser()
    AP.add_argument('-i', '--input', type=argparse.FileType('r'),
        default='marsh_report_raw.txt')
    AP.add_argument('-f', '--freq_mhz', type=float, required=False, default=None)
    ARGS = AP.parse_args()
    MP = MarshReport(ARGS.input.read().split('\n'), ARGS.freq_mhz)
    print MP
