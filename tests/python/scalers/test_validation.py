import numpy as np
import pytest

from flxscalers.scalers._validation import check_array, check_n_features


def test_accepts_nested_lists():
    out = check_array([[0.0, 1.0], [2.0, 3.0]])
    np.testing.assert_array_equal(out, [[0.0, 1.0], [2.0, 3.0]])


def test_coerces_to_float64():
    out = check_array([[1, 2], [3, 4]])
    assert out.dtype == np.float64


def test_1d_input_is_rejected_with_reshape_hint():
    with pytest.raises(ValueError, match="reshape"):
        check_array(np.zeros(3))


@pytest.mark.parametrize("X", [np.zeros((2, 2, 2)), np.zeros(())])
def test_other_non_2d_input_is_rejected(X):
    with pytest.raises(ValueError):
        check_array(X)


@pytest.mark.parametrize("bad_value", [np.nan, np.inf, -np.inf])
def test_non_finite_values_are_rejected(bad_value):
    with pytest.raises(ValueError, match="NaN|infinite"):
        check_array(np.array([[1.0, bad_value], [3.0, 4.0]]))


def test_matching_feature_count_passes_through():
    X = np.array([[1.0, 2.0], [3.0, 4.0]])
    out = check_n_features(X, 2)
    assert out is X


def test_mismatched_feature_count_raises():
    X = np.array([[1.0, 2.0, 3.0]])
    with pytest.raises(ValueError, match="3 features"):
        check_n_features(X, 2)
