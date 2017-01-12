function [dist, flow] = emd(X, Y, X_weights, Y_weights, distance, D)
%Computes the Earth Mover's Distance between two weighted samples
% emd(X, Y[, X_weights, Y_weights, distance, D])
% X : First sample
% Y : Second sample
% X_weights : weights of elements in X (must sum to 1);
%    default: 1/|X| is used as the weight of each element
% Y_weights : weights of elements in Y (must sum to 1);
%    default: 1/|Y| is used as the weight of each element
% distance : valid distance type used by pdist2,
%    or "precomputed" if the pairwise distances D is supplied;
%    default: euclidean
% D : precomputed distance matrix; ignored unless distance="precomputed";
%    must be array of size |X|-by-|Y|
if nargin < 2
    error('Must at least provide two samples.');
end
if nargin < 3
    X_weights = [];
end
if nargin < 4
    Y_weights = [];
end
if nargin < 5
    distance = 'euclidean';
end
if nargin < 6
    D = [];
end

if ~strcmp(distance, 'precomputed')
    n = length(X);
    m = length(Y);
    D = pdist2(X, Y, distance);
    if isempty(X_weights)
        X_weights = ones(1, n)/n;
    elseif n ~= length(X_weights)
        error('Size mismatch of X and X_weights');
    else
        X_weights = reshape(X_weights, 1, []);
    end
    if isempty(Y_weights)
        Y_weights = ones(1, m)/m;
    elseif m ~= length(Y_weights)
        error('Size mismatch of Y and Y_weights');
    else
        Y_weights = reshape(Y_weights, 1, []);
    end
else
    if isempty(D)
        error('D must be supplied when distance=''precomputed''');
    end
    [n, m] = size(D);
    if isempty(X_weights)
        X_weights = ones(1, n)/n;
    elseif n ~= length(X_weights)
        error('Size mismatch of D and X_weights');
    else
        X_weights = reshape(X_weights, 1, []);
    end
    if isempty(Y_weights)
        Y_weights = ones(1, m)/m;
    elseif m ~= length(Y_weights)
        error('Size mismatch of D and Y_weights');
    else
        Y_weights = reshape(Y_weights, 1, []);
    end
end

if nargout > 1
    [dist, flow] = c_emd(X_weights, Y_weights, D);
else
    dist = c_emd(X_weights, Y_weights, D);
end

end
