class Solution {
public:
    int visiblePoints(vector<vector<int>>& points, int angle, vector<int>& location) {
        int answer = 0;
        double pi = acos(-1.0);
        vector<double> pointAngles;
        //iterate through all points p to find the required polar angle with respect to location
        for(auto p : points){
            if(p[0] == location[0] && p[1] == location[1]){
                //these are points with same location as yours
                answer += 1;
            }else {
                int dx = p[0] - location[0];
                int dy = p[1] - location[1];
                double angle = atan2(dy,dx); // gives inverse tangent in radians for a coordinate
                //convert to degrees
                double angle1 = angle * (180) / pi;
                pointAngles.push_back(angle1);
            }
        }
        sort(pointAngles.begin(),pointAngles.end());
        //sorting the angles will make sure that the required count is a contiguous length of angles
        int n = pointAngles.size();
        for(int i= 0; i < n ; ++i) {
            pointAngles.push_back(pointAngles[i] + 360);
            // push the angles again by adding 360 to handle the case of consecutive points being after a complete cycle. ex - 345 and 14 (if angle >= 29)
        }
        int start = 0;
        int cnt = 0;
        // now a simple implementation of sliding window of size angle
        for(int i =0; i < pointAngles.size(); ++i) {
            if(pointAngles[i] - pointAngles[start] > angle){
                start++;
            }
            cnt = max(cnt, i - start + 1);
        }
        answer += cnt; // add this length to the number of overlapping points earlier
        return answer;
    }
};