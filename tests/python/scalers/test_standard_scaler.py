import numpy as np
import pytest

from flxscalers import StandardScaler
from flxscalers import NotFittedError


def test_transform_before_fit_raises():
    scaler = StandardScaler()
    with pytest.raises(NotFittedError):
        scaler.transform(np.array([[1.0], [2.0]]))


def test_inverse_transform_before_fit_raises():
    scaler = StandardScaler()
    with pytest.raises(NotFittedError):
        scaler.inverse_transform(np.array([[1.0], [2.0]]))


def test_fit_transform_centers_and_scales_to_unit_variance():
    X = np.array([[0.0], [5.0], [10.0]])

    out = StandardScaler().fit_transform(X)

    assert out.shape == (3, 1)
    np.testing.assert_allclose(out.ravel(), [-1.2247448714, 0.0, 1.2247448714])
    np.testing.assert_allclose(out.mean(axis=0), [0.0], atol=1e-12)
    np.testing.assert_allclose(out.std(axis=0), [1.0])


def test_each_column_is_standardized_independently():
    # col 0: {1, 3} -> mean 2, std 1;  col 1: {10, 30} -> mean 20, std 10
    X = np.array([[1.0, 10.0], [3.0, 30.0]])

    out = StandardScaler().fit_transform(X)

    np.testing.assert_allclose(out, [[-1.0, -1.0], [1.0, 1.0]])


def test_with_mean_false_divides_by_std_without_centering():
    X = np.array([[0.0], [4.0]])  # mean 2, std 2

    out = StandardScaler(with_mean=False).fit_transform(X)

    np.testing.assert_allclose(out.ravel(), [0.0, 2.0])


def test_with_std_false_centers_without_scaling():
    X = np.array([[1.0], [2.0], [3.0]])  # mean 2

    out = StandardScaler(with_std=False).fit_transform(X)

    np.testing.assert_allclose(out.ravel(), [-1.0, 0.0, 1.0])


def test_with_mean_and_std_false_is_identity():
    X = np.array([[1.0, 2.0], [3.0, 4.0]])

    out = StandardScaler(with_mean=False, with_std=False).fit_transform(X)

    np.testing.assert_allclose(out, X)


def test_constant_column_does_not_divide_by_zero():
    X = np.array([[7.0], [7.0], [7.0]])

    out = StandardScaler().fit_transform(X)

    np.testing.assert_array_equal(out.ravel(), [0.0, 0.0, 0.0])


def test_inverse_transform_undoes_transform():
    X = np.array([[2.0, 100.0], [4.0, 200.0], [6.0, 300.0]])

    scaler = StandardScaler()
    roundtrip = scaler.inverse_transform(scaler.fit_transform(X))

    np.testing.assert_allclose(roundtrip, X, atol=1e-9)


def test_inverse_transform_round_trips_a_constant_column():
    X = np.array([[7.0], [7.0], [7.0]])

    scaler = StandardScaler()
    roundtrip = scaler.inverse_transform(scaler.fit_transform(X))

    np.testing.assert_allclose(roundtrip.ravel(), [7.0, 7.0, 7.0])


def test_transform_applies_statistics_captured_at_fit_time():
    scaler = StandardScaler()
    scaler.fit(np.array([[0.0], [10.0]]))  # mean 5, std 5

    out = scaler.transform(np.array([[5.0], [15.0], [-5.0]]))

    np.testing.assert_allclose(out.ravel(), [0.0, 2.0, -2.0])


def test_fit_transform_preserves_input_shape():
    X = np.arange(1.0, 13.0).reshape(4, 3)

    out = StandardScaler().fit_transform(X)

    assert out.shape == (4, 3)


def test_fit_returns_self():
    scaler = StandardScaler()
    assert scaler.fit(np.array([[0.0], [1.0]])) is scaler


def test_output_is_float64():
    out = StandardScaler().fit_transform(np.array([[1], [2], [3]]))
    assert out.dtype == np.float64


def test_accepts_nested_lists():
    out = StandardScaler().fit_transform([[0.0], [5.0], [10.0]])
    np.testing.assert_allclose(out.ravel(), [-1.2247448714, 0.0, 1.2247448714])


@pytest.mark.parametrize("X", [np.zeros(3), np.zeros((2, 2, 2))])
def test_non_2d_input_is_rejected(X):
    with pytest.raises(ValueError):
        StandardScaler().fit(X)


def test_repr_shows_flags():
    assert repr(StandardScaler(with_mean=False)) == "StandardScaler(with_mean=False, with_std=True)"
